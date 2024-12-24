//
// Generated file, do not edit! Created by opp_msgtool 6.0 from NodeMessage.msg.
//

#ifndef __NODEMESSAGE_M_H
#define __NODEMESSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class NodeMessage;
/**
 * Class generated from <tt>NodeMessage.msg:19</tt> by opp_msgtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet NodeMessage
 * {
 *     \@customize(true);  // see the generated C++ header for more info
 *     int Header;//data seq number
 *     string M_Payload;
 *     string Trailer; // to be converted to string  8<------0
 *     int Frame_Type;//Data=2/ACK=1 /NACK=0
 *     int ACK;// Ack of which frame in the window
 * }
 * </pre>
 *
 * NodeMessage_Base is only useful if it gets subclassed, and NodeMessage is derived from it.
 * The minimum code to be written for NodeMessage is the following:
 *
 * <pre>
 * class NodeMessage : public NodeMessage_Base
 * {
 *   private:
 *     void copy(const NodeMessage& other) { ... }

 *   public:
 *     NodeMessage(const char *name=nullptr, short kind=0) : NodeMessage_Base(name,kind) {}
 *     NodeMessage(const NodeMessage& other) : NodeMessage_Base(other) {copy(other);}
 *     NodeMessage& operator=(const NodeMessage& other) {if (this==&other) return *this; NodeMessage_Base::operator=(other); copy(other); return *this;}
 *     virtual NodeMessage *dup() const override {return new NodeMessage(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from NodeMessage_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(NodeMessage)
 * </pre>
 */
class NodeMessage_Base : public ::omnetpp::cPacket
{
  protected:
    int Header = 0;
    omnetpp::opp_string M_Payload;
    omnetpp::opp_string Trailer;
    int Frame_Type = 0;
    int ACK = 0;

  private:
    void copy(const NodeMessage_Base& other);

  protected:
    bool operator==(const NodeMessage_Base&) = delete;
    // make constructors protected to avoid instantiation

    // make assignment operator protected to force the user override it
    NodeMessage_Base& operator=(const NodeMessage_Base& other);

  public:
    virtual ~NodeMessage_Base();
    NodeMessage_Base(const char *name=nullptr, short kind=0);
    NodeMessage_Base(const NodeMessage_Base& other);
    virtual NodeMessage_Base *dup() const override {throw omnetpp::cRuntimeError("You forgot to manually add a dup() function to class NodeMessage");}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual int getHeader() const;
    virtual void setHeader(int Header);

    virtual const char * getM_Payload() const;
    virtual void setM_Payload(const char * M_Payload);

    virtual const char * getTrailer() const;
    virtual void setTrailer(const char * Trailer);

    virtual int getFrame_Type() const;
    virtual void setFrame_Type(int Frame_Type);

    virtual int getACK() const;
    virtual void setACK(int ACK);
};


namespace omnetpp {

template<> inline NodeMessage_Base *fromAnyPtr(any_ptr ptr) { return check_and_cast<NodeMessage_Base*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __NODEMESSAGE_M_H

