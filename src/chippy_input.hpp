#ifndef CHIPPY_INPUT_HPP
#define CHIPPY_INPUT_HPP

#include <algorithm>
#include <array>

/*
  Our keyboard:
  If a key at any index is 1, then that key is pressed; otherwise not pressed.
  Layout of chippy's keyboard
  Of course we map it differently. I have 60% keyboard :(
   _ _ _ _		 _ _ _ _
  |1|2|3|C|		|1|2|3|4|
  |4|5|6|D|		|Q|W|E|R|
  |7|8|9|E| ->|A|S|D|F|
  |A|0|B|F|		|Z|X|C|V|
   - - - -		 - - - -
*/

class ChippyInput
{
public:
  ChippyInput() {}
  ~ChippyInput() {}

  std::array<int, 16> &GetInput();

private:
  std::array<int, 16> keyboard_{};
};

#endif