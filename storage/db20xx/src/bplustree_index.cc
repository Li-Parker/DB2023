#include "b_plus_tree_index.h"
#include "b_plus_tree_internal_node.h"
#include "b_plus_tree_leaf_node.h"

namespace db20xx{
  /**get
   * */
  bool BplusTreeIndex::get_internal_next_child(db20xx::BPlusTreeNode *current, const db20xx::Key key, db20xx::BPlusTreeNode *&next_child) {
     int size = current->GetSize();
     auto *tmp_current = dynamic_cast<BPlusTreeInternalNode *>(current);
     for(int i = size-1;i>0;i--){
       if(key>=tmp_current->KeyAt(i)){
         next_child = tmp_current->ValueAt(i);
         return true;
       }
       else if(i==1)
       {
         next_child = tmp_current->ValueAt(0);
         return true;
       }
     }
     return false;
   }
  bool BplusTreeIndex::get(const db20xx::Key &key, db20xx::VersionChainHead *&vchain_head) const{
    if(root->root_node_==nullptr)
      return false;
    BPlusTreeNode* tmp = root->root_node_;
    while(!tmp->IsLeafNode())
      get_internal_next_child(tmp,key,tmp);

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
  /**put
   * */
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
      auto* tmp_leaf = new BPlusTreeLeafNode(IndexNodeType::LEAF_NODE,LEAF_NODE_SIZE,
                                             nullptr, nullptr);
      insert_in_leaf(tmp_leaf, key, vchain_head);
      root->root_node_ = tmp_leaf;
      return true;
    }
    std::vector<BPlusTreeInternalNode*> parentList;//记录父亲节点
    BPlusTreeNode* tmp = root->root_node_;
    while(!tmp->IsLeafNode())
    {
      auto *tmp_internal_node = dynamic_cast<BPlusTreeInternalNode *>(tmp);
      parentList.push_back(tmp_internal_node);//父亲节点Push
      get_internal_next_child(tmp,key,tmp);
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
        auto* right_new_node = new BPlusTreeLeafNode(IndexNodeType::LEAF_NODE,LEAF_NODE_SIZE,
                                                      tmp_leaf_node->GetNextNode(),tmp_leaf_node);
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
        if(right_new_node->GetNextNode()!= nullptr)
          right_new_node->GetNextNode()->SetPreNodeId(right_new_node);
        insert_in_parent(parentList,parentList.size()-1,key,tmp_leaf_node,right_new_node);
    }
    return false;
  }
  /**remove
   * */
   bool BplusTreeIndex::remove(const db20xx::Key &key) {
    if (root->root_node_== nullptr)
        return false;
    //记录父亲节点序列及大小
    std::vector<BPlusTreeInternalNode*> parentList;//记录父亲节点
    std::vector<int> parentList_id;
    int parentList_size = 0;

    BPlusTreeNode* cur = root->root_node_;
    while(!cur->IsLeafNode())
    {
        int tmp_size = cur->GetSize();
        auto *tmp_internal_node = dynamic_cast<BPlusTreeInternalNode *>(cur);
        parentList.push_back(tmp_internal_node);//父亲节点Push
        parentList_size++;
        for(int i = tmp_size-1;i>0;i--)
        {
          Key tmp_key = tmp_internal_node->KeyAt(i);
          if(key>=tmp_key){
            cur = tmp_internal_node->ValueAt(i);
            parentList_id.push_back(i);
            break;
          }
          else if(i==1)
          {
            cur = tmp_internal_node->ValueAt(0);
            parentList_id.push_back(0);
          }
        }

      auto *tmp_cur = dynamic_cast<BPlusTreeInternalNode *>(cur);
      parentList.push_back(tmp_cur);
      get_internal_next_child(cur,key,cur);
    }
    /**删除叶子节点中的（key，value）
     **/
    auto *tmp_leaf_cur = dynamic_cast<BPlusTreeLeafNode *>(cur);
    bool remove_key_success = tmp_leaf_cur->RemoveNode(key);
    if(!remove_key_success) // 如果删除失败，如：key不存在
      return false;
    if(tmp_leaf_cur->GetSize()>=tmp_leaf_cur->GetMinSize()) // 叶子节点够满
      return true;
    /***叶子节点过少*/
    //情况一：有左兄弟
    if(tmp_leaf_cur->GetPreNode()!= nullptr){
      BPlusTreeLeafNode* left_brother = tmp_leaf_cur->GetPreNode();
      BPlusTreeInternalNode* tmp_leaf_cur_parent = parentList[parentList_size-1];
      //如果可以合并
      if(left_brother->GetSize()+tmp_leaf_cur->GetSize()<=left_brother->GetMaxSize()){
          int tmp_leaf_cur_size = tmp_leaf_cur->GetSize();
          for(int i = 0;i<tmp_leaf_cur_size;i++)
            left_brother->PutNode(tmp_leaf_cur->KeyAt(i),tmp_leaf_cur->ValueAt(i));
          //维护叶子链表
          left_brother->SetNextNodeId(tmp_leaf_cur->GetNextNode());
          if(tmp_leaf_cur->GetNextNode()!= nullptr)
            tmp_leaf_cur->GetNextNode()->SetPreNodeId(left_brother);
          delete tmp_leaf_cur;
          return remove_internal_entry(tmp_leaf_cur_parent,tmp_leaf_cur_parent->KeyAt(parentList_id[parentList_size-1]),
                                       parentList_size-2,parentList,parentList_id);
      }
      //不可以左右合并，旋转
      else{
          Key left_last_key = left_brother->KeyAt(left_brother->GetSize()-1);
          VersionChainHead* left_last_value = left_brother->ValueAt(left_brother->GetSize()-1);
          if(!left_brother->PopNode())
            return false;
          tmp_leaf_cur->PutNode(left_last_key,left_last_value);
          tmp_leaf_cur_parent->SetKeyAt(parentList_id[parentList_size-1],left_last_key);
          return true;
      }
    }
    //情况二：没有左兄弟而有右兄弟
    else if (tmp_leaf_cur->GetNextNode()!= nullptr){
      BPlusTreeLeafNode* right_brother = tmp_leaf_cur->GetNextNode();
      BPlusTreeInternalNode* tmp_leaf_cur_parent = parentList[parentList_size-1];
      //如果可以合并
      if(right_brother->GetSize()+tmp_leaf_cur->GetSize()<=tmp_leaf_cur->GetMaxSize())
      {
          int right_brother_size = right_brother->GetSize();
          for(int i = 0;i<right_brother_size;i++)
            tmp_leaf_cur->PutNode(right_brother->KeyAt(i),right_brother->ValueAt(i));
          //维护叶子链表
          tmp_leaf_cur->SetNextNodeId(right_brother->GetNextNode());
          if(right_brother->GetNextNode()!= nullptr)
            right_brother->GetNextNode()->SetPreNodeId(tmp_leaf_cur);
          delete right_brother;
          return remove_internal_entry(tmp_leaf_cur_parent,tmp_leaf_cur_parent->KeyAt(1),
                                parentList_size-2,parentList,parentList_id);
      }
      //不可以合并，旋转
      else{
          Key right_first_key = right_brother->KeyAt(0);
          VersionChainHead* right_first_value = right_brother->ValueAt(0);
          if(!right_brother->RemoveNodeByIndex(0))
            return false;
          tmp_leaf_cur->PutNode(right_first_key,right_first_value);
          tmp_leaf_cur_parent->SetKeyAt(1,right_brother->KeyAt(0));//左兄弟不存在==>位置确定:index = 1
          return true;
      }
    }
    //情况三：无左右兄弟，此时该叶子节点作为根节点
    else return true;
   }
   bool BplusTreeIndex::remove_internal_entry(db20xx::BPlusTreeInternalNode *node, db20xx::Key key, int parent_index,
                                              std::vector<BPlusTreeInternalNode *> parentList, std::vector<int> parentList_id) {
    if(!node->RemoveNode(key))
      return false;
    if(root->root_node_==node)
    {
      if(node->GetSize()==1)
      {
          root->root_node_ = node->ValueAt(0);
          delete node;
      }
     return true;
    }
    if(node->GetSize() >= node->GetMinSize())
     return true;
    BPlusTreeInternalNode* node_parent = parentList[parent_index];
    int key_parent_index = parentList_id[parent_index];
    Key key_parent ;
    // parent_index==0表明node没有左兄弟
    if(key_parent_index==0){
      key_parent = node_parent->KeyAt(1);
      auto* node_right_brother = dynamic_cast<BPlusTreeInternalNode *>(node_parent->ValueAt(1));
      //可以和兄弟节点合并
      if(node->GetSize() + node_right_brother->GetSize() + 1 <= node->GetMaxSize()){
          node_right_brother->SetKeyAt(0,key_parent);
          int brother_size = node_right_brother->GetSize();
          for(int i = 0;i<brother_size;i++)
            node->PutNode(node_right_brother->KeyAt(i), node_right_brother->ValueAt(i));
          delete node_right_brother;
          return remove_internal_entry(node_parent,key_parent,parent_index-1,parentList,parentList_id);
      }
      //不可以合并
      else{
          Key right_brother_first_key = node_right_brother->KeyAt(1);
          BPlusTreeNode* right_brother_first_value = node_right_brother->ValueAt(0);
          node->PutNode(key_parent, right_brother_first_value);
          node_right_brother->RemoveNodeByIndex(0);
          node_parent->SetKeyAt(1,right_brother_first_key);
          return true;
      }
    }
    //node有左兄弟
    else{
      key_parent = node_parent->KeyAt(key_parent_index);
      auto* node_left_brother =dynamic_cast<BPlusTreeInternalNode *>(node_parent->ValueAt(key_parent_index - 1));
      //可以和左兄弟合并
      if(node->GetSize() + node_left_brother->GetSize() + 1 <= node_left_brother->GetMaxSize()){
          node->SetKeyAt(0, key_parent);
          int node_size = node->GetSize();
          for(int i = 0;i<node_size;i++)
            node_left_brother->PutNode(node->KeyAt(i), node->ValueAt(i));
          delete node;
          return remove_internal_entry(node_parent, key_parent, parent_index-1,parentList,parentList_id);
      }
      else{
          Key left_brother_last_key = node_left_brother->KeyAt(node_left_brother->GetSize()-1);
          BPlusTreeNode* left_brother_last_value = node_left_brother->ValueAt(node_left_brother->GetSize()-1);
          node->PutNode(key_parent, left_brother_last_value);
          node_left_brother->PopNode();
          node_parent->SetKeyAt(key_parent_index,left_brother_last_key);
          return true;
      }
    }
   }
}