/*
 * LoRaMaDoR (LoRa-based mesh network for hams) project
 * Copyright (c) 2019 PU5EPX
 */

/* Abstract class for application protocols.
 *
 * A protocol class may have up to three main elements:
 *
 * 1) a handler (concrete implementation of a handler()).
 * This method is called when a packet is received by the station,
 * that is, this station is the final destination. This is true
 * even for QC, QB packets (in which this station is not the *only*
 * destination).
 *
 * The class may choose to handle the packet and return a response,
 * or return 0 and pass it on. Packets handled by a protocol are not
 * offered to other Protocols, and normally they are not delivered to
 * application level (i.e. the user won't see it unless in debug mode).
 *
 * See Proto_Ping.cpp for a concrete example. This class answers
 * PONG to PING packets.
 *
 * 2) A modifier (concrete impl. of modify()). This method is called
 * when a packet is to be forwarded by the station, that is, this
 * station is not the *sole* final destination (therefore, QC and QB
 * packets will be offered to handle() and modify(), in this order).
 *
 * The modifier may return a modified packet, or 0 to pass it on.
 * More than one Protocol can modify a packet. The order is not
 * guaranteed, so implementations must take care not to disrupt
 * other protocols' work.
 *
 * See Proto_Rreq.cpp for a concrete example. This class adds our
 * callsign to the payload of a forwarded RREQ or RRSP packet
 * (and also handles a RREQ request).
 *
 * 3) It can generate packets. In order to do this, it must subclass,
 * instantiate and schedule a Task that calls asynchronously some
 * tx method.
 *
 * See Proto_Beacon for a concrete example. In this example, the Task
 * is created only once, and returning a non-zero value effectively
 * reschedules the next packet.
 *
 * Tasks that interact with hardware must be careful not blocking
 * the main loop for too long, in particular when Wi-Fi is on,
 * because the ESP32 supervisor will reset the program if some
 * interruption is not handled in time. Depending on the case,
 * if there is bitbanging involved, etc. the Wi-Fi should stay
 * disabled.
 */ 

#include "Protocol.h"
#include "Network.h"
#include "Packet.h"

HandlerResponse::HandlerResponse(Ptr<Packet> pkt, bool hide_from_user):
	pkt(pkt), hide_from_user(hide_from_user)
{}

HandlerResponse::HandlerResponse():
	pkt(Ptr<Packet>(0)), hide_from_user(false)
{}

Protocol::Protocol(Network *net): net(net)
{
	// Network becomes the owner
	net->add_protocol(this);
}

Protocol::~Protocol()
{
	net = 0;
}

HandlerResponse Protocol::handle(const Packet&)
{
	// by default, does not handle upon receiving
	return HandlerResponse();
}

Ptr<Packet> Protocol::modify(const Packet&)
{
	// by default, does not modify upon forwarding
	return Ptr<Packet>(0);
}
