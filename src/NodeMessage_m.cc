//
// Generated file, do not edit! Created by opp_msgtool 6.0 from NodeMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "NodeMessage_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

NodeMessage_Base::NodeMessage_Base(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

NodeMessage_Base::NodeMessage_Base(const NodeMessage_Base& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

NodeMessage_Base::~NodeMessage_Base()
{
}

NodeMessage_Base& NodeMessage_Base::operator=(const NodeMessage_Base& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void NodeMessage_Base::copy(const NodeMessage_Base& other)
{
    this->Header = other.Header;
    this->M_Payload = other.M_Payload;
    this->Trailer = other.Trailer;
    this->Frame_Type = other.Frame_Type;
    this->ACK = other.ACK;
}

void NodeMessage_Base::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->Header);
    doParsimPacking(b,this->M_Payload);
    doParsimPacking(b,this->Trailer);
    doParsimPacking(b,this->Frame_Type);
    doParsimPacking(b,this->ACK);
}

void NodeMessage_Base::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->Header);
    doParsimUnpacking(b,this->M_Payload);
    doParsimUnpacking(b,this->Trailer);
    doParsimUnpacking(b,this->Frame_Type);
    doParsimUnpacking(b,this->ACK);
}

int NodeMessage_Base::getHeader() const
{
    return this->Header;
}

void NodeMessage_Base::setHeader(int Header)
{
    this->Header = Header;
}

const char * NodeMessage_Base::getM_Payload() const
{
    return this->M_Payload.c_str();
}

void NodeMessage_Base::setM_Payload(const char * M_Payload)
{
    this->M_Payload = M_Payload;
}

const char * NodeMessage_Base::getTrailer() const
{
    return this->Trailer.c_str();
}

void NodeMessage_Base::setTrailer(const char * Trailer)
{
    this->Trailer = Trailer;
}

int NodeMessage_Base::getFrame_Type() const
{
    return this->Frame_Type;
}

void NodeMessage_Base::setFrame_Type(int Frame_Type)
{
    this->Frame_Type = Frame_Type;
}

int NodeMessage_Base::getACK() const
{
    return this->ACK;
}

void NodeMessage_Base::setACK(int ACK)
{
    this->ACK = ACK;
}

class NodeMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_Header,
        FIELD_M_Payload,
        FIELD_Trailer,
        FIELD_Frame_Type,
        FIELD_ACK,
    };
  public:
    NodeMessageDescriptor();
    virtual ~NodeMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(NodeMessageDescriptor)

NodeMessageDescriptor::NodeMessageDescriptor() : omnetpp::cClassDescriptor("NodeMessage", "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

NodeMessageDescriptor::~NodeMessageDescriptor()
{
    delete[] propertyNames;
}

bool NodeMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<NodeMessage_Base *>(obj)!=nullptr;
}

const char **NodeMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = { "customize",  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *NodeMessageDescriptor::getProperty(const char *propertyName) const
{
    if (!strcmp(propertyName, "customize")) return "true";
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int NodeMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int NodeMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_Header
        FD_ISEDITABLE,    // FIELD_M_Payload
        FD_ISEDITABLE,    // FIELD_Trailer
        FD_ISEDITABLE,    // FIELD_Frame_Type
        FD_ISEDITABLE,    // FIELD_ACK
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *NodeMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "Header",
        "M_Payload",
        "Trailer",
        "Frame_Type",
        "ACK",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int NodeMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "Header") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "M_Payload") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "Trailer") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "Frame_Type") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "ACK") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *NodeMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_Header
        "string",    // FIELD_M_Payload
        "string",    // FIELD_Trailer
        "int",    // FIELD_Frame_Type
        "int",    // FIELD_ACK
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **NodeMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *NodeMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int NodeMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    NodeMessage_Base *pp = omnetpp::fromAnyPtr<NodeMessage_Base>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void NodeMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    NodeMessage_Base *pp = omnetpp::fromAnyPtr<NodeMessage_Base>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'NodeMessage_Base'", field);
    }
}

const char *NodeMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    NodeMessage_Base *pp = omnetpp::fromAnyPtr<NodeMessage_Base>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string NodeMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    NodeMessage_Base *pp = omnetpp::fromAnyPtr<NodeMessage_Base>(object); (void)pp;
    switch (field) {
        case FIELD_Header: return long2string(pp->getHeader());
        case FIELD_M_Payload: return oppstring2string(pp->getM_Payload());
        case FIELD_Trailer: return oppstring2string(pp->getTrailer());
        case FIELD_Frame_Type: return long2string(pp->getFrame_Type());
        case FIELD_ACK: return long2string(pp->getACK());
        default: return "";
    }
}

void NodeMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    NodeMessage_Base *pp = omnetpp::fromAnyPtr<NodeMessage_Base>(object); (void)pp;
    switch (field) {
        case FIELD_Header: pp->setHeader(string2long(value)); break;
        case FIELD_M_Payload: pp->setM_Payload((value)); break;
        case FIELD_Trailer: pp->setTrailer((value)); break;
        case FIELD_Frame_Type: pp->setFrame_Type(string2long(value)); break;
        case FIELD_ACK: pp->setACK(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'NodeMessage_Base'", field);
    }
}

omnetpp::cValue NodeMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    NodeMessage_Base *pp = omnetpp::fromAnyPtr<NodeMessage_Base>(object); (void)pp;
    switch (field) {
        case FIELD_Header: return pp->getHeader();
        case FIELD_M_Payload: return pp->getM_Payload();
        case FIELD_Trailer: return pp->getTrailer();
        case FIELD_Frame_Type: return pp->getFrame_Type();
        case FIELD_ACK: return pp->getACK();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'NodeMessage_Base' as cValue -- field index out of range?", field);
    }
}

void NodeMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    NodeMessage_Base *pp = omnetpp::fromAnyPtr<NodeMessage_Base>(object); (void)pp;
    switch (field) {
        case FIELD_Header: pp->setHeader(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_M_Payload: pp->setM_Payload(value.stringValue()); break;
        case FIELD_Trailer: pp->setTrailer(value.stringValue()); break;
        case FIELD_Frame_Type: pp->setFrame_Type(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_ACK: pp->setACK(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'NodeMessage_Base'", field);
    }
}

const char *NodeMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr NodeMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    NodeMessage_Base *pp = omnetpp::fromAnyPtr<NodeMessage_Base>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void NodeMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    NodeMessage_Base *pp = omnetpp::fromAnyPtr<NodeMessage_Base>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'NodeMessage_Base'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

