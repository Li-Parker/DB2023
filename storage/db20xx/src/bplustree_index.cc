#include "b_plus_tree_index.h"
#include "b_plus_tree_internal_node.h"
#include "b_plus_tree_leaf_node.h"

namespace db20xx{
  bool BplusTreeIndex::get(const db20xx::Key &key, db20xx::VersionChainHead *&vchain_head) const{
    if(root->root_node_==nullptr)
      return false;
    BPlusTreeNode* tmp = root->root_node_;
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
    //TODO ==> 使用二分法改进搜索叶子节点
    for(int j = 0;j<leaf_node_size;j++)
    {
      if(key==tmp_leaf_node->KeyAt(j)){
        vchain_head = tmp_leaf_node->ValueAt(j);
        return true;
      }
    }
    return false;
  }
  bool BplusTreeIndex::insert_in_leaf(db20xx::BPlusTreeLeafNode *leaf_node, db20xx::Key key, db20xx::VersionChainHead *value) {
    return leaf_node->PutNode(key, value);
  }
  bool BplusTreeIndex::put(const db20xx::Key &key, db20xx::VersionChainHead *vchain_head) {
    //如果根节点为空
    if(root->root_node_==nullptr)
    {
      auto* tmp_leaf = new BPlusTreeLeafNode(IndexNodeType::LEAF_NODE,LEAF_NODE_SIZE,1, nullptr);
      insert_in_leaf(tmp_leaf, key, vchain_head);
      root->root_node_ = tmp_leaf;
      return true;
    }
    BPlusTreeNode* tmp = root->root_node_;
    while(!tmp->IsLeafNode())
    {
      int tmp_size = tmp->GetSize();
      auto *tmp_internal_node = dynamic_cast<BPlusTreeInternalNode *>(tmp);

      tmp = tmp_internal_node->ValueAt(0);
      for(int i = tmp_size-1;i>0;i--)
      {
        Key tmp_key = tmp_internal_node->KeyAt(i);
        if(tmp_key<=key){
          tmp = tmp_internal_node->ValueAt(i);
          break;
        }
      }
    }
    int leaf_node_size = tmp->GetSize();
    int leaf_node_max_size = tmp->GetMaxSize();
    //如果叶子节点没有满
    if(leaf_node_size<leaf_node_max_size){
      auto *tmp_leaf_node = dynamic_cast<BPlusTreeLeafNode *>(tmp);
      insert_in_leaf(tmp_leaf_node, key, vchain_head);
    }
    //如果叶子节点满了
    else{

    }
    return false;
  }
}