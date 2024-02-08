#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 4100)
#include "Packet.gen.hpp"

#ifdef __UNREAL__
#include "Network/Packet.h"
#elif __SERVER__
#include "core/Packet.hpp"
#include "util/Types.hpp"
#endif

#include <vector>

/* Additional headers. */


namespace gen {
    enum EMoveState : uint16 {
        MOVE_STATE_NONE,
		MOVE_STATE_IDLE,
		MOVE_STATE_RUN
    };
}
#pragma warning(pop)
