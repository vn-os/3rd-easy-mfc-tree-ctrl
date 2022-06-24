#pragma once

#include <string>
#include <functional>

struct Node
{
  Node(const std::string& name, void* ptr_data = nullptr)
    : m_ptr_data(ptr_data), m_ptr_tv(nullptr)
  {
    std::wstring s(name.cbegin(), name.cend());
    m_name = s.c_str();
  }

  Node(const CString& name, void* ptr_data = nullptr)
    : m_name(name), m_ptr_data(ptr_data), m_ptr_tv(nullptr) {}
  virtual ~Node() {}

  Node& operator=(const Node& right)
  {
    m_name     = right.m_name;
    m_ptr_tv   = right.m_ptr_tv;
    m_ptr_data = right.m_ptr_data;
    return *this;
  }

  CString m_name;
  TVITEM* m_ptr_tv;
  void*   m_ptr_data;
};

class EasyTreeCtrl : public CTreeCtrl
{
  DECLARE_DYNAMIC(EasyTreeCtrl)

  enum class eNotifyType
  {
    SELECTING,
    BOX_CHECKING,

    BEFORE_INSERTING,
    AFTER_INSERTING,

    BEFORE_MODIFYING,
    AFTER_MODIFYING,

    BEFORE_DELETING,
    AFTER_DELETING,
  };

public:
  EasyTreeCtrl(CDialog* pParent = nullptr);
  virtual ~EasyTreeCtrl();

  void Populate(std::function<void(HTREEITEM& root)> pfn, const CString& name = L"<root>");
  void Iterate(HTREEITEM pItem, std::function<void(HTREEITEM pItem)> pfn);
  HTREEITEM InsertNode(HTREEITEM& pParent, Node* pNode);
  void Clear();

public:
  void OnNotify(std::function<bool(eNotifyType action, Node* pNode)> pfn);

protected:
  virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
  afx_msg void OnDestroy();

  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void OnContextMenuHandler(UINT ID);

  afx_msg void OnLClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);

  afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);

  afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

  DECLARE_MESSAGE_MAP()

private:
  HTREEITEM GetpItemFocusing(UINT* pFlags = nullptr);
  bool EditItem(HTREEITEM pItem);
  bool Notify(eNotifyType action, HTREEITEM pItem);
  bool Notify(eNotifyType action, Node* pNode);
  void Cleanup(HTREEITEM pItem);

private:
  CDialog* m_pParent;
  std::function<bool(eNotifyType action, Node* pNode)> m_pfnOnNotify;
};
