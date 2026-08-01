#include "AudioListener.h"

namespace Good
{

    AudioListener& AudioListener::Get()
    {
        static AudioListener s_Instance;
        return s_Instance;
    }

} // namespace Good