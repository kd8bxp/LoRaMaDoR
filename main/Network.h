/*
 * LoRaMaDoR (LoRa-based mesh network for hams) project
 * Copyright (c) 2019 PU5EPX
 */

// Main LoRaMaDoR network class, plus some auxiliary types

#ifndef __NETWORK_H
#define __NETWORK_H

#include <cstddef>
#include <cstdint>
#include "Vector.h"
#include "Dict.h"
#include "Task.h"
#include "Params.h"
#include "Callsign.h"

#define SECONDS 1000
#define MINUTES 60000

class Protocol;
class Packet;

struct Neighbour {
	Neighbour(int rssi, int32_t timestamp):
		rssi(rssi), timestamp(timestamp) {}
	Neighbour() {}
	int rssi;
	int32_t timestamp;
};

struct RecvLogItem {
	RecvLogItem(int rssi, int32_t timestamp):
		rssi(rssi), timestamp(timestamp) {}
	RecvLogItem() {}
	int rssi;
	int32_t timestamp;
};

class Network {
public:
	Network(const Callsign &callsign);
	virtual ~Network();

	Callsign me() const;
	void send(const Callsign &to, Params params, const Buffer& msg);
	void run_tasks(uint32_t);
	Dict<Neighbour> neigh() const;
	static uint32_t fudge(uint32_t avg, double fudge);

	// publicised to bridge with uncoupled code
	void radio_recv(const char *recv_area, size_t plen, int rssi);
	size_t get_last_pkt_id() const;
	size_t get_next_pkt_id();

	// publicised to be called by Protocols
	void schedule(Task*);

	// Called by Protocol base class constructor.
	// Never call this yourself.
	void add_protocol(Protocol*);

	// publicised to be called by Tasks
	uint32_t tx(const Buffer&);
	void forward(Ptr<Packet>, bool, uint32_t);
	uint32_t clean_recv_log(uint32_t);
	uint32_t clean_neigh(uint32_t);

	// publicised for testing purposes
	TaskManager& _task_mgr();
	Dict<Neighbour>& _neighbours();
	Dict<RecvLogItem>& _recv_log();

private:
	void recv(Ptr<Packet> pkt);
	void sendmsg(const Ptr<Packet> pkt);

	Callsign my_callsign;
	TaskManager task_mgr;
	Dict<Neighbour> neighbours;
	Dict<RecvLogItem> recv_log;
	size_t last_pkt_id;
	Vector< Ptr<Protocol> > protocols;
};

#endif
