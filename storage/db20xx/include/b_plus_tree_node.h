#pragma once
#include <cstdint>
#include "utils.h"
namespace db20xx {

#define DB20XX_NODE_SIZE 4096

/**
枚举类 IndexNodeType，它包含了三个枚举值：
INVALID_INDEX_NODE：无效的索引节点，值为 0
LEAF_NODE：叶子节点
INTERNAL_NODE：内部节点
*/
enum class IndexNodeType { INVALID_INDEX_NODE = 0, LEAF_NODE, INTERNAL_NODE };

/**
 * Both internal and leaf node are inherited from this node.
 *
 * It actually serves as a header part for each B+ tree node and
 * contains information shared by both leaf node and internal node.
 *
 * Header format (size in byte):
 * ----------------------------------------------------------------------------
 * | nodeType (4) | rwlock(sizeof(std::shared_mutex)) | CurrentSize (4) | MaxSize (4) |
 * ----------------------------------------------------------------------------
 */
class BPlusTreeNode {
 public:
  // Delete all constructor / destructor to ensure memory safety
  //删除了默认构造函数，表示该类必须使用自定义的构造函数来创建对象。
  BPlusTreeNode() = delete; 
  //删除了复制构造函数，表示该类不能被复制（复制构造函数用于从一个已有对象初始化另一个新的对象）。
  BPlusTreeNode(const BPlusTreeNode &other) = delete;
  //删除了析构函数，表示该类的对象不能被直接删除或销毁。
  //  ~BPlusTreeNode() = delete;
  virtual ~BPlusTreeNode() {} // 确保虚函数表被正确构造

  /**
   * 1.auto 关键字指代函数的返回类型将由编译器自动推导得出，推导的方式是根据函数体中 return 语句返回的类型来确定函数的实际返回类型。
   * 箭头符号 “->” 后面指定了返回类型为 bool。
   * 2.相对于传统的函数声明方式，返回类型后置语法带来了更灵活的声明方式，可以使用更复杂的类型，
   * 例如模板类型、嵌套类型等，同时还可以明确规定函数的返回类型。
   * 此外，使用返回类型后置语法的函数声明更加简洁明了，可读性更强。
  */
  auto IsLeafNode() const -> bool;
  void SetNodeType(IndexNodeType node_type);

  auto GetSize() const -> int;
  void SetSize(int size);
  void IncreaseSize(int amount);

  auto GetMaxSize() const -> int;
  void SetMaxSize(int max_size);
  auto GetMinSize() const -> int;

 private:
  // member variable, attributes that both internal and leaf node share
  // 使用 __attribute__((__unused__)) 属性告诉编译器，即使这个成员变量在代码中未被使用，也不要产生未使用变量的警告信息。
  // 这样可以使代码更加干净，不受编译器的冗余警告干扰。需要注意的是，这个 attribute 并不是标准 C++ 的语法，而是 GCC 扩展的语法。
  IndexNodeType node_type_ __attribute__((__unused__));
  ReaderWriterLatch rwlock_;
  int size_ __attribute__((__unused__));
  int max_size_ __attribute__((__unused__));
};

}
