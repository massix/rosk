// Test serialization and de-serialization of messages

#include <iostream>
#include <rosk_message.hpp>
#include <version1.pb.h>

int main()
{
	rosk::com::rosk_message rmsg("LOGIN");

	rmsg.access_internal_message<rosk::com::login_message>().set_client_id("c_id");
	rmsg.access_internal_message<rosk::com::login_message>().set_wished_robo_type("TANK");
	rmsg.commit();

	rosk::com::login_message const & mymsg(rmsg.get_internal_message<rosk::com::login_message>());
	if (mymsg.client_id() != "c_id") return -1;
	if (mymsg.wished_robo_type() != "TANK") return -1;

	return 0;
}

