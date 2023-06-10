#include "b_plus_tree_index.h"
#include "b_plus_tree_internal_node.h"
#include "b_plus_tree_leaf_node.h"

namespace db20xx{
  bool BplusTreeIndex::get(const db20xx::Key &key, db20xx::VersionChainHead *&vchain_head) const{
    if(root==nullptr)
      return false;
    BPlusTreeNode* tmp = root;
    while(!tmp->IsLeafNode())
    {
      int tmp_size = tmp->GetSize();
      auto *tmp_internal_node = dynamic_cast<BPlusTreeInternalNode *>(tmp);
      for(int i = tmp_size-1;i>0;i--)
      {
        Key tmp_key = tmp_internal_node->KeyAt(i);
        if(tmp_key<=key){
          tmp = tmp_internal_node->ValueAt(i);
          break;
        }
        else if(i==1){
          tmp = tmp_internal_node->ValueAt(0);
        }
      }
    }
    auto *tmp_leaf_node = dynamic_cast<BPlusTreeLeafNode *>(tmp);
    int leaf_node_size = tmp_leaf_node->GetSize();
    for(int j = 0;j<leaf_node_size;j++)
    {
      if(key==tmp_leaf_node->KeyAt(j)){
        vchain_head = tmp_leaf_node->ValueAt(j);
        return true;
      }
    }
    return false;
  }
}