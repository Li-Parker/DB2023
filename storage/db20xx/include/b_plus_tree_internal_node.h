#pragma once

#include <queue>
#include <string>

#include "b_plus_tree_node.h"
#include "index.h"
#include "version_chain.h"

namespace db20xx {

using InteralNodeMappingType = std::pair<Key, BPlusTreeNode*>;
#define INTERNAL_NODE_HEADER_SIZE 12
#define INTERNAL_NODE_SIZE ((DB20XX_NODE_SIZE - INTERNAL_NODE_HEADER_SIZE) / (sizeof(InteralNodeMappingType)))

/**
 * Store n indexed keys and n+1 child pointers (node_address) within internal node.
 * Pointer NODE_ID(i) points to a subtree in which all keys K satisfy:
 * K(i) <= K < K(i+1).
 * NOTE: since the number of keys does not equal to number of child pointers,
 * the first key always remains invalid. That is to say, any search/lookup
 * should ignore the first key.
 *
 * Internal node format (keys are stored in increasing order):
 *  --------------------------------------------------------------------------
 * | HEADER | KEY(1)+NODE_ID(1) | KEY(2)+NODE_ID(2) | ... | KEY(n)+NODE_ID(n) |
 *  --------------------------------------------------------------------------
 */

class BPlusTreeInternalNode : public BPlusTreeNode {
 public:
  // Deleted to disallow initialization

  BPlusTreeInternalNode() = delete;
  BPlusTreeInternalNode(const BPlusTreeInternalNode &other) = delete;
  BPlusTreeInternalNode(IndexNodeType node_type,int max_size);
  BPlusTreeInternalNode(IndexNodeType node_type,int max_size,Key key,BPlusTreeNode* left_node,BPlusTreeNode* right_node);
/**
 * '@param' 是一种文档注释标记，用于描述函数的一个参数或多个参数。
 * 这种文档注释标记还包括其他常用标记，如 '@return'（描述函数返回值）、
 * '@note'（提供一些额外的注意事项）、'@warning'（指出一些潜在的风险或问题）、
 * '@throw'（描述函数可能抛出的异常类型）等。
 * 通常，这些文档注释标记会被文档生成工具识别，并用于生成 API 文档，以便其他开发者使用。
*/

  /**
   * Writes the necessary header information to a newly created page, must be called after
   * the creation of a new page to make a valid BPlusTreeInternalNode
   * @param max_size Maximal size of the page
   */
  void Init(int max_size = INTERNAL_NODE_SIZE);

  /**
   * @param index The index of the key to get. Index must be non-zero.
   * @return Key at index
   */
  auto KeyAt(int index) const -> KeyType;

  /**
   *
   * @param index The index of the key to set. Index must be non-zero.
   * @param key The new value for key
   */
  void SetKeyAt(int index, const KeyType &key);

  /**
   *
   * @param value the value to search for
   */
  auto ValueIndex(const InternalNodeValueType &value) const -> int;

  /**
   *
   * @param index the index
   * @return the value at the index
   */
  auto ValueAt(int index) const -> InternalNodeValueType;

  /**
   * @brief For test only, return a string representing all keys in
   * this internal page, formatted as "(key1,key2,key3,...)"
   *
   * @return std::string
   */
  auto ToString() const -> std::string {
    std::string kstr = "(";
    bool first = true;

    // first key of internal page is always invalid
    for (int i = 1; i < GetSize(); i++) {
      KeyType key = KeyAt(i);
      if (first) {
        first = false;
      } else {
        kstr.append(",");
      }

      kstr.append(key.s, key.len);
    }
    kstr.append(")");

    return kstr;
  }
  bool PutNode(Key key, BPlusTreeNode* node);
  bool PopNode();
 private:
  // Flexible array member for page data.
  std::vector<InteralNodeMappingType> array_;
};
}  // namespace db20xx

