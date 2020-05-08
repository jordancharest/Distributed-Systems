// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: swim.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_swim_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_swim_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_swim_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_swim_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_swim_2eproto;
namespace swim {
class Member;
class MemberDefaultTypeInternal;
extern MemberDefaultTypeInternal _Member_default_instance_;
class Message;
class MessageDefaultTypeInternal;
extern MessageDefaultTypeInternal _Message_default_instance_;
}  // namespace swim
PROTOBUF_NAMESPACE_OPEN
template<> ::swim::Member* Arena::CreateMaybeMessage<::swim::Member>(Arena*);
template<> ::swim::Message* Arena::CreateMaybeMessage<::swim::Message>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace swim {

enum Member_Status : int {
  Member_Status_ALIVE = 0,
  Member_Status_SUSPECTED = 1,
  Member_Status_FAILED = 2,
  Member_Status_Member_Status_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  Member_Status_Member_Status_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool Member_Status_IsValid(int value);
constexpr Member_Status Member_Status_Status_MIN = Member_Status_ALIVE;
constexpr Member_Status Member_Status_Status_MAX = Member_Status_FAILED;
constexpr int Member_Status_Status_ARRAYSIZE = Member_Status_Status_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Member_Status_descriptor();
template<typename T>
inline const std::string& Member_Status_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Member_Status>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Member_Status_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Member_Status_descriptor(), enum_t_value);
}
inline bool Member_Status_Parse(
    const std::string& name, Member_Status* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Member_Status>(
    Member_Status_descriptor(), name, value);
}
enum Message_Type : int {
  Message_Type_PING = 0,
  Message_Type_ACK = 1,
  Message_Type_Message_Type_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  Message_Type_Message_Type_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool Message_Type_IsValid(int value);
constexpr Message_Type Message_Type_Type_MIN = Message_Type_PING;
constexpr Message_Type Message_Type_Type_MAX = Message_Type_ACK;
constexpr int Message_Type_Type_ARRAYSIZE = Message_Type_Type_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Message_Type_descriptor();
template<typename T>
inline const std::string& Message_Type_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Message_Type>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Message_Type_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Message_Type_descriptor(), enum_t_value);
}
inline bool Message_Type_Parse(
    const std::string& name, Message_Type* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Message_Type>(
    Message_Type_descriptor(), name, value);
}
// ===================================================================

class Member :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:swim.Member) */ {
 public:
  Member();
  virtual ~Member();

  Member(const Member& from);
  Member(Member&& from) noexcept
    : Member() {
    *this = ::std::move(from);
  }

  inline Member& operator=(const Member& from) {
    CopyFrom(from);
    return *this;
  }
  inline Member& operator=(Member&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Member& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Member* internal_default_instance() {
    return reinterpret_cast<const Member*>(
               &_Member_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Member& a, Member& b) {
    a.Swap(&b);
  }
  inline void Swap(Member* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Member* New() const final {
    return CreateMaybeMessage<Member>(nullptr);
  }

  Member* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Member>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Member& from);
  void MergeFrom(const Member& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Member* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "swim.Member";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_swim_2eproto);
    return ::descriptor_table_swim_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef Member_Status Status;
  static constexpr Status ALIVE =
    Member_Status_ALIVE;
  static constexpr Status SUSPECTED =
    Member_Status_SUSPECTED;
  static constexpr Status FAILED =
    Member_Status_FAILED;
  static inline bool Status_IsValid(int value) {
    return Member_Status_IsValid(value);
  }
  static constexpr Status Status_MIN =
    Member_Status_Status_MIN;
  static constexpr Status Status_MAX =
    Member_Status_Status_MAX;
  static constexpr int Status_ARRAYSIZE =
    Member_Status_Status_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  Status_descriptor() {
    return Member_Status_descriptor();
  }
  template<typename T>
  static inline const std::string& Status_Name(T enum_t_value) {
    static_assert(::std::is_same<T, Status>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function Status_Name.");
    return Member_Status_Name(enum_t_value);
  }
  static inline bool Status_Parse(const std::string& name,
      Status* value) {
    return Member_Status_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kStatusFieldNumber = 1,
    kIdFieldNumber = 2,
    kPortFieldNumber = 3,
  };
  // .swim.Member.Status status = 1;
  void clear_status();
  ::swim::Member_Status status() const;
  void set_status(::swim::Member_Status value);
  private:
  ::swim::Member_Status _internal_status() const;
  void _internal_set_status(::swim::Member_Status value);
  public:

  // uint32 id = 2;
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 port = 3;
  void clear_port();
  ::PROTOBUF_NAMESPACE_ID::uint32 port() const;
  void set_port(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_port() const;
  void _internal_set_port(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:swim.Member)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  int status_;
  ::PROTOBUF_NAMESPACE_ID::uint32 id_;
  ::PROTOBUF_NAMESPACE_ID::uint32 port_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_swim_2eproto;
};
// -------------------------------------------------------------------

class Message :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:swim.Message) */ {
 public:
  Message();
  virtual ~Message();

  Message(const Message& from);
  Message(Message&& from) noexcept
    : Message() {
    *this = ::std::move(from);
  }

  inline Message& operator=(const Message& from) {
    CopyFrom(from);
    return *this;
  }
  inline Message& operator=(Message&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Message& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Message* internal_default_instance() {
    return reinterpret_cast<const Message*>(
               &_Message_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(Message& a, Message& b) {
    a.Swap(&b);
  }
  inline void Swap(Message* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Message* New() const final {
    return CreateMaybeMessage<Message>(nullptr);
  }

  Message* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Message>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Message& from);
  void MergeFrom(const Message& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Message* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "swim.Message";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_swim_2eproto);
    return ::descriptor_table_swim_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef Message_Type Type;
  static constexpr Type PING =
    Message_Type_PING;
  static constexpr Type ACK =
    Message_Type_ACK;
  static inline bool Type_IsValid(int value) {
    return Message_Type_IsValid(value);
  }
  static constexpr Type Type_MIN =
    Message_Type_Type_MIN;
  static constexpr Type Type_MAX =
    Message_Type_Type_MAX;
  static constexpr int Type_ARRAYSIZE =
    Message_Type_Type_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  Type_descriptor() {
    return Message_Type_descriptor();
  }
  template<typename T>
  static inline const std::string& Type_Name(T enum_t_value) {
    static_assert(::std::is_same<T, Type>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function Type_Name.");
    return Message_Type_Name(enum_t_value);
  }
  static inline bool Type_Parse(const std::string& name,
      Type* value) {
    return Message_Type_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kMemberFieldNumber = 3,
    kTypeFieldNumber = 1,
    kSenderIdFieldNumber = 2,
  };
  // repeated .swim.Member member = 3;
  int member_size() const;
  private:
  int _internal_member_size() const;
  public:
  void clear_member();
  ::swim::Member* mutable_member(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::swim::Member >*
      mutable_member();
  private:
  const ::swim::Member& _internal_member(int index) const;
  ::swim::Member* _internal_add_member();
  public:
  const ::swim::Member& member(int index) const;
  ::swim::Member* add_member();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::swim::Member >&
      member() const;

  // .swim.Message.Type type = 1;
  void clear_type();
  ::swim::Message_Type type() const;
  void set_type(::swim::Message_Type value);
  private:
  ::swim::Message_Type _internal_type() const;
  void _internal_set_type(::swim::Message_Type value);
  public:

  // uint32 sender_id = 2;
  void clear_sender_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 sender_id() const;
  void set_sender_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_sender_id() const;
  void _internal_set_sender_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:swim.Message)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::swim::Member > member_;
  int type_;
  ::PROTOBUF_NAMESPACE_ID::uint32 sender_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_swim_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Member

// .swim.Member.Status status = 1;
inline void Member::clear_status() {
  status_ = 0;
}
inline ::swim::Member_Status Member::_internal_status() const {
  return static_cast< ::swim::Member_Status >(status_);
}
inline ::swim::Member_Status Member::status() const {
  // @@protoc_insertion_point(field_get:swim.Member.status)
  return _internal_status();
}
inline void Member::_internal_set_status(::swim::Member_Status value) {
  
  status_ = value;
}
inline void Member::set_status(::swim::Member_Status value) {
  _internal_set_status(value);
  // @@protoc_insertion_point(field_set:swim.Member.status)
}

// uint32 id = 2;
inline void Member::clear_id() {
  id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Member::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Member::id() const {
  // @@protoc_insertion_point(field_get:swim.Member.id)
  return _internal_id();
}
inline void Member::_internal_set_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  id_ = value;
}
inline void Member::set_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:swim.Member.id)
}

// uint32 port = 3;
inline void Member::clear_port() {
  port_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Member::_internal_port() const {
  return port_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Member::port() const {
  // @@protoc_insertion_point(field_get:swim.Member.port)
  return _internal_port();
}
inline void Member::_internal_set_port(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  port_ = value;
}
inline void Member::set_port(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_port(value);
  // @@protoc_insertion_point(field_set:swim.Member.port)
}

// -------------------------------------------------------------------

// Message

// .swim.Message.Type type = 1;
inline void Message::clear_type() {
  type_ = 0;
}
inline ::swim::Message_Type Message::_internal_type() const {
  return static_cast< ::swim::Message_Type >(type_);
}
inline ::swim::Message_Type Message::type() const {
  // @@protoc_insertion_point(field_get:swim.Message.type)
  return _internal_type();
}
inline void Message::_internal_set_type(::swim::Message_Type value) {
  
  type_ = value;
}
inline void Message::set_type(::swim::Message_Type value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:swim.Message.type)
}

// uint32 sender_id = 2;
inline void Message::clear_sender_id() {
  sender_id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Message::_internal_sender_id() const {
  return sender_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Message::sender_id() const {
  // @@protoc_insertion_point(field_get:swim.Message.sender_id)
  return _internal_sender_id();
}
inline void Message::_internal_set_sender_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  sender_id_ = value;
}
inline void Message::set_sender_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_sender_id(value);
  // @@protoc_insertion_point(field_set:swim.Message.sender_id)
}

// repeated .swim.Member member = 3;
inline int Message::_internal_member_size() const {
  return member_.size();
}
inline int Message::member_size() const {
  return _internal_member_size();
}
inline void Message::clear_member() {
  member_.Clear();
}
inline ::swim::Member* Message::mutable_member(int index) {
  // @@protoc_insertion_point(field_mutable:swim.Message.member)
  return member_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::swim::Member >*
Message::mutable_member() {
  // @@protoc_insertion_point(field_mutable_list:swim.Message.member)
  return &member_;
}
inline const ::swim::Member& Message::_internal_member(int index) const {
  return member_.Get(index);
}
inline const ::swim::Member& Message::member(int index) const {
  // @@protoc_insertion_point(field_get:swim.Message.member)
  return _internal_member(index);
}
inline ::swim::Member* Message::_internal_add_member() {
  return member_.Add();
}
inline ::swim::Member* Message::add_member() {
  // @@protoc_insertion_point(field_add:swim.Message.member)
  return _internal_add_member();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::swim::Member >&
Message::member() const {
  // @@protoc_insertion_point(field_list:swim.Message.member)
  return member_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace swim

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::swim::Member_Status> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::swim::Member_Status>() {
  return ::swim::Member_Status_descriptor();
}
template <> struct is_proto_enum< ::swim::Message_Type> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::swim::Message_Type>() {
  return ::swim::Message_Type_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_swim_2eproto
