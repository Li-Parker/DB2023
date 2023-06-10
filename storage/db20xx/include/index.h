#pragma once
#include <cstdint>
#include "b_plus_tree_node.h"
#include "record.h"
#include "transaction.h"
#include "utils.h"
#include "version_chain.h"
#include "thread_context.h"
#include "masstree-beta/str.hh"

namespace db20xx {

using namespace Masstree::lcdf;
typedef Str Key;

/**using 是 C++11 中引入的一个关键字，用来定义类型别名。
 * */
using KeyType = Key;
using InternalNodeValueType = BPlusTreeNode *;
using LeafNodeValueType = VersionChainHead *;

struct KeyInfo {
  /**
    mysql keypart counted from 1,
    db20xx keypart counted from 0;
  */
  void add_key_part(uint32_t key_part) { key_parts.push_back(key_part - 1); }
  uint32_t get_key_length() { return key_len; }

  /**表示一个数据库表的 schema (或者说结构或模式)，例如表中包含哪些列、每个列的数据类型等信息。
   * */
  Schema schema;
  
  /**
   * 表示一个索引所使用的 keyparts，每个 keypart 表示索引使用的一个列的位置。
   * 在这里，我们约定索引使用的列的位置从0 开始计数，但 MySQL 中从第一个列开始计数，
   * 所以在 add_key_part 函数中，参数 key_part 需要从 1 开始计数。
   * 例如，一个索引是 (col1, col2) ，则 key_parts 中存放的是 [0, 1] 。
   * */
  std::vector<int> key_parts;
  uint32_t key_len = 0; //key length capacity
};

//TODO
class ScanIterator {
public:
  const Key &get_current_key() {
    // TODO
    assert(false);
    return *new Key;
  }

  VersionChainHead *get_current_value() {
    // TODO
    assert(false);
    return nullptr;
  }

  void reset() {
    // TODO
    assert(false);
    return;
  }
};

class Index {
  /**
   * friend class 表示将一个类（这里是 Table）声明为另一个类（这里是 Index）的友元类，即允许友元类可以访问这个类的私有成员。
   * 在下面的代码中，friend class Table 将 Table 类声明为 Index 类的友元类，
   * 因此 Table 类可以访问 Index 类中的私有成员变量和私有成员函数。
   * */
  friend class Table;

 public:
  Index(void) {}
  Index(const KeyInfo &keyinfo) : keyinfo_(keyinfo) {}
  ~Index() {}

  virtual bool get(const Key &key, VersionChainHead *&vchain_head) const = 0;

  virtual bool put(const Key &key, VersionChainHead *vchain_head) = 0;

  virtual bool remove(const Key &key) = 0;

  virtual bool scan_range_first(const Key &key, VersionChainHead *&vchain_head,
                        bool emit_firstkey, ScanIterator &iter) const = 0;

  virtual bool scan_range_next(VersionChainHead *&vchain_head, ScanIterator &iter) const = 0;

  virtual bool rscan_range_first(const Key &key, VersionChainHead *&vchain_head,
                         bool emit_firstkey, ScanIterator &iter) const = 0;

  virtual bool rscan_range_next(VersionChainHead *&vchain_head, ScanIterator &iter) const = 0;

  /**
  @brief
    build key from a db20xx record
    have memory allocation in this function, need to free by others
  @args
    arg1 record: record payload, without record header
  */
  void build_key(const char *record, Key &output_key, ThreadContext *thd_ctx) {
    char *key_data = thd_ctx->get_key_container();
    char *key_cursor = key_data;
    uint32_t key_len = 0;
    for (auto i : keyinfo_.key_parts) {
      const Field &field = keyinfo_.schema.get_field(i);
      const char *field_data = nullptr;
      uint32_t data_len = 0;

      field.get_field_data(record, field_data, data_len);
      memcpy(key_cursor, field_data, data_len);
      key_cursor += data_len;
      key_len += data_len;
    }

    output_key.s = key_data;
    output_key.len = key_len;
  }

  void build_key_from_mysql_record(const char *mysql_record, Key &output_key, ThreadContext *thd_ctx) {
    char *key_data = thd_ctx->get_key_container();
    char *key_cursor = key_data;
    uint32_t key_len = 0;
    for (auto i: keyinfo_.key_parts) {
      const Field &field = keyinfo_.schema.get_field(i);
      const char *field_data = nullptr;
      uint32_t data_len = 0;

      field.get_mysql_field_data(mysql_record, field_data, data_len);
      memcpy(key_cursor, field_data, data_len);
      key_cursor += data_len;
      key_len += data_len;
    }
    output_key.s = key_data;
    output_key.len = key_len;
  }

  uint32_t get_key_length() { return keyinfo_.get_key_length(); }
  const KeyInfo &get_key_info() const { return keyinfo_; }

 protected:
  KeyInfo keyinfo_;
};


}  // namespace db20xx
