#include "Controls.h"

#pragma comment(lib, "comctl32.lib")

using namespace autownd;

Listbox::Listbox()
{
}

Listbox::~Listbox()
{
}

void Listbox::clear()
{
	SendMessage(wnd(), LB_RESETCONTENT, 0, 0);
}

Listbox::LBSet Listbox::operator[](int i)
{
	return LBSet(wnd(), i);
}

Listbox::LBSet Listbox::operator*()
{
	int pos = (int)SendMessage(wnd(), LB_GETCURSEL, 0, 0);
	return LBSet(wnd(), pos);
}

Listbox::LBSet Listbox::operator<<(const TCHAR * text)
{
	int pos = (int)SendMessage(wnd(), LB_ADDSTRING, 0, (LPARAM)(text));
	return LBSet(wnd(), pos);
}

Listbox::LBSet Listbox::insert(int i, const TCHAR *text)
{
	int pos = (int)SendMessage(wnd(), LB_ADDSTRING, i, (LPARAM)(text));
	return LBSet(wnd(), pos);
}

Listbox::LBSet::LBSet(HWND listbox, int id)
	:theID(id), theListbox(listbox)
{
}

Listbox::LBSet::~LBSet()
{
}

int Listbox::LBSet::getData()
{
	if (theListbox && theID != -1)	return (int)SendMessage(theListbox, LB_GETITEMDATA, theID, 0);
	else return 0;
}

void Listbox::LBSet::setData(int data)
{
	if (theListbox && theID != -1) SendMessage(theListbox, LB_SETITEMDATA, theID, data);
}

/////////////////////////////////////////////////

List::LSet::LSet(HWND list, int row)
	:theList(list), theItem(nullptr)
{
	theItem = new LVITEM;
	ZeroMemory(theItem, sizeof(LVITEM));

	theItem->iItem = row;
}

List::LSet::~LSet()
{
	if (theItem) delete theItem;
}

void List::LSet::update()
{
	if (theItem == nullptr || theList == 0) return;
	theItem->iItem = ListView_InsertItem(theList, theItem);
}

const LPLVITEM List::LSet::sync()
{
	ListView_GetItem(theList, theItem);
	return theItem;
}

List::LSet & List::LSet::setText(TCHAR * text, size_t size)
{
	theItem->mask |= LVIF_TEXT;
	theItem->cchTextMax = size;
	theItem->pszText = text;
	return *this;
}

List::LSet & List::LSet::setText(int column, TCHAR * text)
{
	ListView_SetItemText(theList, theItem->iItem, column, text);
	return *this;
}

List::LSet & List::LSet::setImage(int image)
{
	theItem->mask |= LVIF_IMAGE;
	theItem->iImage = image;

	return *this;
}

List::LSet & List::LSet::setParam(LPARAM param)
{
	theItem->mask |= LVIF_PARAM;
	theItem->lParam = param;

	return *this;
}

List::LSet & List::LSet::getText(TCHAR * text, size_t size, int column)
{
	ListView_GetItemText(theList, theItem->iItem, column, text, size);
	return *this;
}

List::LSet & List::LSet::getRect(LPRECT rect, int column)
{
	ListView_GetSubItemRect(theList, theItem->iItem, column, LVIR_BOUNDS, rect);
	return *this;
}

List::List()
	:theImages(0)
{
}

List::~List()
{
	if (theImages) ImageList_Destroy(theImages);
}

void List::buildImageList(int cx, int cy)
{
	theImages = ImageList_Create(cx, cy, ILC_MASK, 1, 1);

	//ListView_SetImageList(theLeft->wnd(), theImages, LVSIL_NORMAL);
	ListView_SetImageList(wnd(), theImages, LVSIL_SMALL);

}

int List::addIcon(int id)
{
	if (theImages == 0) return -1;

	HICON hiconItem = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id));
	if (hiconItem == nullptr) return -1;

	int re = ImageList_AddIcon(theImages, hiconItem);
	DestroyIcon(hiconItem);

	return re;
}

void List::clear()
{
	ListView_DeleteAllItems(wnd());
}

List::LSet List::at(int i)
{
	return LSet(wnd(), i);
}

List::LSet List::selection()
{
	int iPos = ListView_GetNextItem(wnd(), -1, LVNI_SELECTED);

	return LSet(wnd(), iPos);
}

void autownd::List::extendStyle(long style)
{
	ListView_SetExtendedListViewStyle(wnd(), style);
}

List::LColumn List::addColumn(int i)
{
	return LColumn(wnd(), i);
}

void autownd::List::resizeColumn(int column, int size)
{
	ListView_SetColumnWidth(wnd(), column, size);
}

int List::getCount()
{
	return ListView_GetItemCount(wnd());
}

List::LColumn::LColumn(HWND list, int id)
	:theList(list), theID(id)
{
	theColumn = new LVCOLUMN;
	ZeroMemory(theColumn, sizeof(LVCOLUMN));
}

List::LColumn::~LColumn()
{
	delete theColumn;
}

List::LColumn & List::LColumn::set(TCHAR * text, size_t size)
{
	theColumn->mask |= LVCF_TEXT;
	theColumn->cchTextMax = size;
	theColumn->pszText = text;

	return *this;
}

List::LColumn & List::LColumn::set(int width)
{
	theColumn->mask |= LVCF_WIDTH;
	theColumn->cx = width;

	return *this;
}

void List::LColumn::update()
{
	ListView_InsertColumn(theList, theID, theColumn);
}

////////////////////////
Tree::Tree()
{
}

Tree::~Tree()
{
	TreeView_DeleteAllItems(wnd());
}

HTREEITEM Tree::addItem(HTREEITEM parent, const TCHAR * item, LPARAM lParam)
{
	TVINSERTSTRUCT tvins;

	tvins.item.mask = TVIF_TEXT | TVIF_PARAM;//| TVIF_IMAGE | TVIF_SELECTEDIMAGE |;

											 // Set the text of the item. 
	tvins.item.pszText = const_cast<TCHAR*>(item);
	tvins.item.cchTextMax = wcslen(item);

	//image
	tvins.item.iImage = 0;
	tvins.item.iSelectedImage = 0;

	// Save the heading level in the item's application-defined data area. 
	tvins.item.lParam = lParam;
	tvins.hInsertAfter = (HTREEITEM)TVI_FIRST;

	// Set the parent item based on the specified level. 
	if (parent == NULL)
		tvins.hParent = TVI_ROOT;
	else
		tvins.hParent = parent;

	// Add the item to the tree-view control. 
	HTREEITEM treeItem = (HTREEITEM)SendMessage(wnd(), TVM_INSERTITEM,
		0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
	if (parent)
		TreeView_Expand(wnd(), parent, TVE_EXPAND); //expand the parent if exist

	return treeItem;
}

void Tree::removeSelection()
{
	TreeView_DeleteItem(wnd(), selection());
}

LPARAM Tree::getSelectedData()
{
	TVITEM item;
	item.mask = TVIF_PARAM;
	ZeroMemory(&item, sizeof(item));
	item.hItem = TreeView_GetSelection(wnd());
	TreeView_GetItem(wnd(), &item);

	return item.lParam;
}

HTREEITEM Tree::selection()
{
	return TreeView_GetSelection(wnd());
}

////////////////////////////

Edit* Edit::theActive = nullptr;
WNDPROC Edit::theOldEditProc = nullptr;

autownd::Edit::Edit()
	:theWnd(nullptr), theRecv(nullptr)
{

}

autownd::Edit::~Edit()
{
	destroy();
}

void autownd::Edit::destroy()
{
	DestroyWindow(theWnd);

	theWnd = nullptr;
	theActive = nullptr;

}

void autownd::Edit::confirmEdit()
{
	if (theRecv == nullptr) return;
	TCHAR buff[255];
	GetWindowText(theWnd, buff, 255);

	theRecv->handleMsg({ { "buff",buff } });
	destroy();
}

int autownd::Edit::init(memory::ParamChain params)
{

	HWND parent = nullptr;
	LPRECT rect = nullptr;
	TCHAR *buff = nullptr;

	memory::find(params, "parent", parent);
	memory::find(params, "rect", rect);
	memory::find(params, "buff", buff);


	//create wnd
	HWND edit = CreateWindowEx(0, L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL
		, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, parent, 0, GetModuleHandle(0), 0);

	if (edit == nullptr)
	{
		return GetLastError();
	}

	if (theActive || theWnd) destroy();
	theWnd = edit;

	HFONT font = (HFONT)SendMessage(parent, WM_GETFONT, 0, 0);
	SendMessage(edit, WM_SETFONT, (WPARAM)font, FALSE);

	if (buff) SetWindowText(edit, buff);
	ShowWindow(edit, SW_SHOW);
	SetFocus(edit);

	theOldEditProc = (WNDPROC)SetWindowLongPtr(edit, GWLP_WNDPROC, (LONG_PTR)subEditProc);

	theActive = this;

	return 0;

}

void autownd::Edit::setRecv(IMsgProcess * recv)
{
	theRecv = recv;
}

LRESULT autownd::Edit::subEditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (theActive == nullptr) return TRUE;

	switch (msg)
	{
	case WM_KILLFOCUS:
	{
		theActive->destroy();
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_RETURN:
			theActive->confirmEdit();
			return 0;
		case VK_ESCAPE:
			theActive->destroy();
			return 0;
		}
	}
	break;
	}
	return CallWindowProc(theOldEditProc, wnd, msg, wParam, lParam);
}

autownd::ContextMenu::ContextMenu()
{
	thePopup = CreatePopupMenu();
}

autownd::ContextMenu::~ContextMenu()
{
	DeleteObject(thePopup);
}

void autownd::ContextMenu::addMenuItem(TCHAR * itemname, UINT_PTR param, UINT pos)
{
	InsertMenu(thePopup, pos, MF_BYPOSITION | MF_STRING, param, itemname);
}

void autownd::ContextMenu::show(int x, int y, HWND parent)
{
	RECT rect; GetWindowRect(parent, &rect);
	SetForegroundWindow(parent);
	TrackPopupMenu(thePopup, TPM_TOPALIGN | TPM_LEFTALIGN, rect.left + x, rect.top + y, 0, parent, NULL);
}

///////////////////////////////


FileDialog::FileDialog()
{
	ZeroMemory(&theData, sizeof(OPENFILENAME));
	theData.lStructSize = OPENFILENAME_SIZE_VERSION_400;
	thePath.resize(255);
}

FileDialog::~FileDialog()
{
}

void autownd::FileDialog::open(memory::ParamChain params)
{
	theData.hwndOwner = GetForegroundWindow();												
	theData.lpstrFilter = TEXT("Xml文件(*.xml)\0*.xml\0所有文件(*.*)\0*.*\0\0");
	theData.lpstrFile = &thePath[0];             
	theData.nMaxFile = thePath.size();     
	theData.lpstrTitle = L"Open...";      
	theData.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST /*| OFN_ALLOWMULTISELECT 允许选择多个文件*/;

	if (!GetOpenFileName(&theData)) return;

}

void autownd::FileDialog::save(memory::ParamChain params)
{
	theData.hwndOwner = GetForegroundWindow();          // 拥有者句柄      
	theData.lpstrFilter = TEXT("Xml文件(*.xml)\0*.xml\0所有文件(*.*)\0*.*\0\0");
	theData.lpstrFile = &thePath[0];
	theData.nMaxFile = thePath.size();
	theData.lpstrTitle = L"Save...";
	theData.Flags = OFN_OVERWRITEPROMPT;        // 覆盖提示  

	if(!GetSaveFileName(&theData)) return;
	
}
