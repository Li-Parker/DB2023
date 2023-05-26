#include "b_plus_tree_leaf_node.h"

namespace db20xx
{
    void BPlusTreeLeafNode::Init(int max_size = LEAF_NODE_SIZE){
        SetSize(max_size);
    }
    auto BPlusTreeLeafNode::GetNextNode() const -> BPlusTreeLeafNode*{
        return next_node_;
    }
    void BPlusTreeLeafNode::SetNextNodeId(BPlusTreeLeafNode *next_node_id){
        next_node_ = next_node_id;
    }
    auto BPlusTreeLeafNode::KeyAt(int index) const -> KeyType{
        if (index<0)
        {
            std::cout << "Error:invalid index: "<< index << std::endl;
        }
        else
        {
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
    }
}