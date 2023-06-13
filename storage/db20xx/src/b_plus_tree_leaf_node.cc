#include "b_plus_tree_leaf_node.h"

namespace db20xx
{
    BPlusTreeLeafNode::BPlusTreeLeafNode(db20xx::IndexNodeType node_type, int max_size, int size, db20xx::BPlusTreeLeafNode *next_node)
    :BPlusTreeNode(node_type, max_size, size) {
      next_node_ = next_node;
    }
    void BPlusTreeLeafNode::Init(int max_size){
      SetMaxSize(max_size);
    }
    auto BPlusTreeLeafNode::GetNextNode() const -> BPlusTreeLeafNode*{
        return next_node_;
    }
    void BPlusTreeLeafNode::SetNextNodeId(BPlusTreeLeafNode *next_node_id){
        next_node_ = next_node_id;
    }
    auto BPlusTreeLeafNode::KeyAt(int index) const -> KeyType{
        assert(array_[index].first);
            /**
             * `.first`是`std::pair`模板类的一个公共成员，它用于访问`std::pair`中存储的第一个元素。
             * 在C++中，`std::pair`是一个模板类，用于表示包含两个值的有序对。
             * `.first`成员变量是一个公共成员，用于访问这个有序对的第一个元素。
             * 在本例中，`MappingType`是一个`std::pair`类型，
             * 其中包含两个元素，第一个元素是`Key`类型的数据（一个自定义类型的别名），
             * 第二个元素是`VersionChainHead*`指针类型的数据。
             * 因此，当我们使用`.first`来访问`MappingType`的第一个元素时，
             * 它实际上访问的是该有序对中存储的`Key`类型的数据。
            */
        return array_[index].first;
    }

    auto BPlusTreeLeafNode::ValueAt(int index) const -> VersionChainHead* {
        assert(array_[index].second!=nullptr);
        return array_[index].second;
    }
/**叶子节点不满的时候才可以调用该函数
 * */
    bool BPlusTreeLeafNode::PutNode(db20xx::Key key, db20xx::VersionChainHead *value) {
        if(GetSize()<GetMaxSize()){
          int num = GetSize();
          int index = 0;
          while(index<num){
            if(array_[index].first==key)
            {
              array_[index].second = value;
              return true;
            }
            if(array_[index].first>key)
              break;
            index++;
          }
          array_.insert(array_.begin()+index, LeafNodeMappingType(key, value));
          IncreaseSize(1);
          return true;
        }
        return false;
    }
    void BPlusTreeLeafNode::PopNode() {
        array_.pop_back();
    }
}