#pragma once
#include "WndProgram.h"
#include <CommCtrl.h>

namespace autownd
{
	class Listbox
		:public WndObj
	{
	public:
		class LBSet
		{
		public:
			LBSet(HWND listbox, int id);
			~LBSet();

			int getData();
			void setData(int data);

		private:
			HWND theListbox;
			int theID;
		};

		Listbox();
		~Listbox();

		void clear();

		LBSet operator[](int i);
		LBSet operator*();
		LBSet operator<<(const TCHAR *text);
		LBSet insert(int i, const TCHAR *text);

	private:
	};

	//////////////////////////////////////////////

	class List
		:public WndObj
	{
	public:
		class LSet
		{
		public:
			LSet(HWND list, int row);
			~LSet();

			void update(bool isNew = true);
			const LPLVITEM sync();

			LSet& setText(TCHAR * text, size_t size);
			LSet& setText(int column, TCHAR * text);
			LSet& setImage(int image);
			LSet& setParam(LPARAM param);

			LSet& getText(TCHAR * text, size_t size, int column);
			LSet& getRect(LPRECT rect, int column);

		private:
			HWND theList;
			LPLVITEM theItem;
		};

		class LColumn
		{
		public:
			LColumn(HWND list, int id);
			~LColumn();

			LColumn& set(TCHAR *text, size_t size);
			LColumn& set(int width);

			void update();

		private:
			HWND theList;
			LPLVCOLUMN theColumn;
			int theID;
		};

		List();
		~List();

		void buildImageList(int cx, int cy);
		int addIcon(int id);

		void clear();

		LSet at(int i = eoi);
		LSet selection();
		void extendStyle(long style);

		int getNextSelectedItem(int i);

		LColumn addColumn(int i);
		void resizeColumn(int column, int size = LVSCW_AUTOSIZE);

		int getCount();

		//the end 
		static const int eoi = 2147483647;

	private:
		HIMAGELIST theImages;
	};

	//////////////////////
	class Tree
		:public WndObj
	{
	public:
		Tree();
		~Tree();

		HTREEITEM addItem(HTREEITEM parent, const TCHAR * item, LPARAM lParam = 0);

		void removeSelection();

		LPARAM getSelectedData();

		HTREEITEM selection();

	};
	
	///////////////////////
	class Edit
	{
	public:
		Edit();
		~Edit();

		void destroy();
		void confirmEdit();

		int init(HWND parent, RECT *rect, TCHAR *buff, IMsgProcess * recv);
		HWND wnd() { return theWnd; }

	private:
		HWND theWnd;
		IMsgProcess * theRecv;

		static Edit * theActive;
		static WNDPROC theOldEditProc;
		static LRESULT CALLBACK subEditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

	};

	///////////////////
	class ContextMenu
	{
	public:
		ContextMenu();
		~ContextMenu();

		void addMenuItem(TCHAR* itemname, UINT_PTR param, UINT pos = -1);

		void show(int x, int y, HWND parent);

	private:
		HMENU thePopup;
	};

	/////////////////
	class FileDialog
	{
	public:
		FileDialog();
		~FileDialog();

		void open(memory::ParamChain params);
		void save(memory::ParamChain params);

		const std::wstring & getPath() { return thePath; }

	private:
		OPENFILENAME theData;
		std::wstring thePath;
	};

}