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
  void BplusTreeIndex::insert_in_parent(std::vector<BPlusTreeInternalNode *> parentList, unsigned parent_id,
                                        db20xx::Key key, db20xx::BPlusTreeNode *left,db20xx::BPlusTreeNode *right) {
    if(left==root->root_node_){
      auto * new_root = new BPlusTreeInternalNode(IndexNodeType::INTERNAL_NODE,INTERNAL_NODE_SIZE,
                                                 key,left,right);
      root->root_node_ = new_root;
      return;
    }
    BPlusTreeInternalNode* left_parent = parentList[parent_id];
    if(!left_parent->IsFull())
    {
      left_parent->PutNode(key,right);
      return;
    }
    else{
      int left_parent_size = left_parent->GetSize();
      int new_right_size = left_parent_size/2;
      int new_left_size = left_parent_size-new_right_size;
      auto * new_right_brother = new BPlusTreeInternalNode(IndexNodeType::INTERNAL_NODE,INTERNAL_NODE_SIZE);
      for(int i = 0;i<new_right_size;i++)
      {
        new_right_brother->PutNode(left_parent->KeyAt(new_left_size+i),
                                   left_parent->ValueAt(new_left_size+i));
      }
      for(int j = 0;j<new_right_size;j++)
        left_parent->PopNode();
      if(key<new_right_brother->KeyAt(0))
        left_parent->PutNode(key,right);
      else
        new_right_brother->PutNode(key,right);
      insert_in_parent(parentList,parent_id-1,new_right_brother->KeyAt(0),
                       left_parent,new_right_brother);
    }
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
    std::vector<BPlusTreeInternalNode*> parentList;//记录父亲节点
    BPlusTreeNode* tmp = root->root_node_;
    while(!tmp->IsLeafNode())
    {
      int tmp_size = tmp->GetSize();
      auto *tmp_internal_node = dynamic_cast<BPlusTreeInternalNode *>(tmp);
      parentList.push_back(tmp_internal_node);//父亲节点Push
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
    auto *tmp_leaf_node = dynamic_cast<BPlusTreeLeafNode *>(tmp);
    //如果叶子节点没有满
    if(leaf_node_size<leaf_node_max_size){
      insert_in_leaf(tmp_leaf_node, key, vchain_head);
    }
    //如果叶子节点满了
    else{
        int full_size = tmp->GetSize();
        int right_brother_size = full_size/2;
        auto* right_new_node = new BPlusTreeLeafNode(IndexNodeType::LEAF_NODE,LEAF_NODE_SIZE,right_brother_size, nullptr);
        //与课本上的算法不同，这里是先分裂再插入新节点
        // step1:分裂
        for(int i = 0;i<right_brother_size;i++)
          right_new_node->PutNode(tmp_leaf_node->KeyAt(full_size-right_brother_size+i),
                                  tmp_leaf_node->ValueAt(full_size-right_brother_size+i));

        for(int j = 0;j<right_brother_size;j++)
          tmp_leaf_node->PopNode();
        // step2:插入新节点
        if(key>=right_new_node->KeyAt(0))
          right_new_node->PutNode(key, vchain_head);
        else
          tmp_leaf_node->PutNode(key, vchain_head);
        tmp_leaf_node->SetNextNodeId(right_new_node);

        insert_in_parent(parentList,parentList.size()-1,key,tmp_leaf_node,right_new_node);
    }
    return false;
  }
}