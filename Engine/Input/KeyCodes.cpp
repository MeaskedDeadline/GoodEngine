#include "KeyCodes.h"

namespace Good
{

    const char* KeyToString(Key key)
    {
        switch (key)
        {
        case Key::A: return "A"; case Key::B: return "B"; case Key::C: return "C";
        case Key::D: return "D"; case Key::E: return "E"; case Key::F: return "F";
        case Key::G: return "G"; case Key::H: return "H"; case Key::I: return "I";
        case Key::J: return "J"; case Key::K: return "K"; case Key::L: return "L";
        case Key::M: return "M"; case Key::N: return "N"; case Key::O: return "O";
        case Key::P: return "P"; case Key::Q: return "Q"; case Key::R: return "R";
        case Key::S: return "S"; case Key::T: return "T"; case Key::U: return "U";
        case Key::V: return "V"; case Key::W: return "W"; case Key::X: return "X";
        case Key::Y: return "Y"; case Key::Z: return "Z";
        case Key::Space:      return "Space";
        case Key::Enter:      return "Enter";
        case Key::Escape:     return "Escape";
        case Key::Tab:        return "Tab";
        case Key::Backspace:  return "Backspace";
        case Key::Left:       return "Left";
        case Key::Right:      return "Right";
        case Key::Up:         return "Up";
        case Key::Down:       return "Down";
        case Key::LeftShift:  return "LeftShift";
        case Key::RightShift: return "RightShift";
        case Key::LeftCtrl:   return "LeftCtrl";
        case Key::RightCtrl:  return "RightCtrl";
        case Key::LeftAlt:    return "LeftAlt";
        case Key::F1:  return "F1";  case Key::F2:  return "F2";
        case Key::F3:  return "F3";  case Key::F4:  return "F4";
        case Key::F5:  return "F5";  case Key::F6:  return "F6";
        case Key::F7:  return "F7";  case Key::F8:  return "F8";
        case Key::F9:  return "F9";  case Key::F10: return "F10";
        case Key::F11: return "F11"; case Key::F12: return "F12";
        default: return "Unknown";
        }
    }

} // namespace Good