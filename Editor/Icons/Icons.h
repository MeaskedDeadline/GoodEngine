#pragma once
#include "../../Engine/Core/Core.h"

namespace Good
{

class Icons
{
public:
    static const char* Folder()      { return "[DIR]";  }
    static const char* File()        { return "[FILE]"; }
    static const char* Texture()     { return "[IMG]";  }
    static const char* Mesh()        { return "[MESH]"; }
    static const char* Audio()       { return "[SND]";  }
    static const char* Scene()       { return "[SCN]";  }
    static const char* Script()      { return "[SCR]";  }
    static const char* Material()    { return "[MAT]";  }
    static const char* Play()        { return "[ > ]";  }
    static const char* Pause()       { return "[ || ]"; }
    static const char* Stop()        { return "[ [] ]"; }
    static const char* Add()         { return "[+]";    }
    static const char* Remove()      { return "[-]";    }
    static const char* Duplicate()   { return "[=]";    }
    static const char* Eye()         { return "[O]";    }
    static const char* EyeOff()      { return "[X]";    }
    static const char* Lock()        { return "[L]";    }
    static const char* Settings()    { return "[*]";    }
    static const char* Refresh()     { return "[R]";    }
    static const char* Search()      { return "[?]";    }
    static const char* Save()        { return "[S]";    }
    static const char* Open()        { return "[O]";    }
    static const char* Translate()   { return "[T]";    }
    static const char* Rotate()      { return "[R]";    }
    static const char* Scale()       { return "[S]";    }
    static const char* World()       { return "[W]";    }
    static const char* Local()       { return "[L]";    }
    static const char* Camera()      { return "[C]";    }
    static const char* Light()       { return "[*]";    }
    static const char* Entity()      { return "[E]";    }
    static const char* Component()   { return "[C]";    }
    static const char* Warning()     { return "[!]";    }
    static const char* Error_()      { return "[X]";    }
    static const char* Info()        { return "[i]";    }
};

} // namespace Good