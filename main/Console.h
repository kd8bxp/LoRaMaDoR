#ifndef __CONSOLE_H
#define __CONSOLE_H

#include "Packet.h"
#include "Network.h"
#include "Display.h"
#include "ArduinoBridge.h"
#include "CLI.h"

class Network;

void cons_setup(Ptr<Network> net);
void cons_handle();
void cons_telnet_enable();
void cons_telnet_disable();

// Receive keystrokes from Telnet
void cons_telnet_type(char c);

// Goes either to Telnet session if open, or serial
void console_print(const char *);
void console_print(char);
void console_println(const char *);
void console_println();

// Goes straight to serial
void serial_print(const char *);
void serial_println(const char *);

#endif