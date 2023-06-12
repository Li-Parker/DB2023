#include "b_plus_tree_node.h"

/**
 * namespace 是 C++ 中一种用于定义命名空间的关键字，
 * 它可以用来将一组相关的函数、类和变量分组，并将它们封装在一个命名空间内。
 * 命名空间可以避免不同库、不同组件或者不同文件之间的命名冲突，使得不同组件之间的相同名称可以共存，并且可以被简单地区分。
*/
namespace db20xx {
/*
 * Helper methods to get/set node type
 * Node type enum class is defined in b_plus_tree_node.h
 */
BPlusTreeNode::BPlusTreeNode(db20xx::IndexNodeType node_type, int max_size, int size) {
  node_type_ = node_type;
  max_size_ = max_size;
  size_ = size;
}

auto BPlusTreeNode::IsLeafNode() const -> bool {
    return  node_type_ == IndexNodeType::LEAF_NODE;
    }
void BPlusTreeNode::SetNodeType(IndexNodeType node_type) {
    node_type_ = node_type;
}

/*
 * Helper methods to get/set size (number of key/value pairs stored in that
 * node)
 */
auto BPlusTreeNode::GetSize() const -> int { return size_; }
void BPlusTreeNode::SetSize(int size) { size_ = size; }
void BPlusTreeNode::IncreaseSize(int amount) { size_ = size_ + amount; }

/*
 * Helper methods to get/set max size (capacity) of the node
 */
auto BPlusTreeNode::GetMaxSize() const -> int { return max_size_; }
void BPlusTreeNode::SetMaxSize(int size) { max_size_ = size;}

/*
 * Helper method to get min node size
 * Generally, min node size == max node size / 2
 */
auto BPlusTreeNode::GetMinSize() const -> int {
    if(node_type_ == IndexNodeType::LEAF_NODE)
      return max_size_/2;
    else
      return (max_size_+1)/2;
}

}  // namespace db20xx

