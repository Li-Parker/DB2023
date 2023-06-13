#include "index.h"
#include "b_plus_tree_leaf_node.h"
#include "b_plus_tree_internal_node.h"
#include "b_plus_tree_header_node.h"
#include <deque>

namespace db20xx {

class BplusTreeIndex : public Index {
  friend class Table;

 public:
  BplusTreeIndex(void) {}
  /**
   *BplusTreeIndex(const KeyInfo &keyinfo):Index(keyinfo){}是 BplusTreeIndex类的构造函数，使用初始化列表进行初始化。
   * 初始化列表中的 Index(keyinfo) 表示调用 Index 类的构造函数，并将 keyinfo 参数传递给父类的构造函数，从而初始化 Index 类型的部分成员变量。
   * 其中 Index(keyinfo) 是在 BplusTreeIndex 类的构造函数定义时写入的，它表示显式调用了父类 Index 构造函数，并传递了一个参数 keyinfo。
   * 这种写法可以在子类构造函数中通过初始化列表将参数传递到父类的构造函数中，从而完成两个类之间的构造函数关联，这也是C++中实现继承的常见方法之一。
   * */
  BplusTreeIndex(const KeyInfo &keyinfo) : Index(keyinfo) {}
  ~BplusTreeIndex() {}


  /**
    @brief
      given a key, get the value
    @return values
      @retval1 true: key exists
      @retval2 false: key doesnot exist
  */
  bool get([[maybe_unused]] const Key &key, [[maybe_unused]] VersionChainHead *&vchain_head) const override;
  static bool insert_in_leaf(BPlusTreeLeafNode* leaf_node,Key key,VersionChainHead* value);
  bool insert_in_parent(std::vector<BPlusTreeInternalNode*> parentList,int parent_id, Key key, BPlusTreeNode* value);
  /**
  @brief
    put a key-value pair to B+ tree

    [[maybe_unused]] 是一个C++11引入的新特性，它可以用来消除未使用变量或函数的编译器警告。
    在代码中，我们有时候会定义一些变量或函数，并没有在后续的代码中使用，但编译器仍然会为其分配内存空间，这就会造成一定的浪费。
    为了解决这个问题，C++11中引入了[[maybe_unused]]关键字，可以告诉编译器不必发出相关警告，因为我们知道这个函数或者变量也许不会被使用。
    使用它可以在代码中实现一定的变量或函数注释，同时避免了不必要的编译器警告。
  */
  bool put([[maybe_unused]] const Key &key, [[maybe_unused]] VersionChainHead *vchain_head) override;

  /** @remove a <key,value> pair from b+ tree */
  bool remove([[maybe_unused]] const Key &key) override {
    //TODO
    return false;
  }

  /** @Given a key, locate to the first tuple greater or equal to key, depending on emit_firstkey.
   * And save the scan state into ScanIterator */
  bool scan_range_first([[maybe_unused]] const Key &key, [[maybe_unused]] VersionChainHead *&vchain_head,
                        [[maybe_unused]] bool emit_firstkey, [[maybe_unused]] ScanIterator &iter) const override {
    //TODO
    return false;
  }

  /** @Use iterator to get the next value. If no next value, return false. */
  bool scan_range_next([[maybe_unused]] VersionChainHead *&vchain_head, [[maybe_unused]] ScanIterator &iter) const override {
    //TODO
    return false;
  }

  /** @reverse direction */
  bool rscan_range_first([[maybe_unused]] const Key &key, [[maybe_unused]] VersionChainHead *&vchain_head,
                         [[maybe_unused]] bool emit_firstkey, [[maybe_unused]] ScanIterator &iter) const override {
    //TODO
    return false;
  }

  /** @reverse direction */
  bool rscan_range_next([[maybe_unused]] VersionChainHead *&vchain_head, [[maybe_unused]] ScanIterator &iter) const override {
    //TODO
    return false;
  }

 private:
  BPlusTreeHeaderNode* root;
//  BPlusTreeNode * root;
  //TODO:
};
}
