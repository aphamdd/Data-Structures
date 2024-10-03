#pragma once
#include "LinkedList.h"
#include <iostream>

// TODO: is there a way to skip having to pass in my text through LinkedList->LLNode
LinkedList::LinkedList(sf::RenderWindow& win, sf::Text& text) :
  window(win),
  LLText(text),
  cursor(0.f, 3) {
  cursor.setFillColor(sf::Color::White);
  cursor.setOutlineThickness(1.5f);
  cursor.setOutlineColor(sf::Color::Black);

  try {
    if (!nodeTexture.loadFromFile("./Textures/linkedlistframe.png"))
      throw std::runtime_error("linked list node texture error");
    if (!cursorTexture.loadFromFile("./Textures/linkedlistframe.png"))
      throw std::runtime_error("cursor texture error");
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void LinkedList::clear() {
  while (head)
    head = std::move(head->next);
  rawptr.active = nullptr;
  rawptr.currAnimate = nullptr;
  head = nullptr;
  rawptr.prev = nullptr;
  rawptr.tail = nullptr;
}

// TODO: don't return midway of a statement, return at the end
// so I can shift any intersecting node
int LinkedList::add() {
  // create head
  if (!head) {
    sf::Vector2f pos(75.f, 400.f); // random ahh position
    head = std::make_unique<LLNode>(pos, LLText, nodeTexture);
    head->sprite.setColor(sf::Color::Red);
    rawptr.tail = head.get();
  }
  // insert in front of active ptr
  else if (rawptr.active) {
    if (rawptr.active->next) {
      sf::Vector2f pos = rawptr.active->sprite.getPosition();
      pos.x += rawptr.active->size.x + 10.f;
      std::unique_ptr<LLNode> newNode = std::make_unique<LLNode>(pos, LLText, nodeTexture);
      newNode->next = std::move(rawptr.active->next);
      rawptr.active->next = std::move(newNode);
      if (rawptr.active != head.get())
        rawptr.active->sprite.setColor(sf::Color::White);
      rawptr.active = rawptr.active->next.get();
      updatePrevPtr(rawptr.active);
      if (rawptr.active != rawptr.tail)
        rawptr.active->sprite.setColor(sf::Color::Green);
      rawptr.active->data += rawptr.prev->data;
      rawptr.active->updateLine(rawptr.prev);
      rawptr.active->next.get()->updateLine(rawptr.active);
      rawptr.active->setText(rawptr.active->data);
      // TODO: I don't like having a loop like this, fix it. recursion?
      // bug: only if rawptr.active->next, needs to apply when ANY node is added
      //while (!shiftForward(rawptr.active, rawptr.active->next.get())) {
        //continue;
      //}
    }
    else if (!rawptr.active->next) {
      addTail();
      if (rawptr.active != head.get())
        rawptr.active->sprite.setColor(sf::Color::White);
      rawptr.active = rawptr.active->next.get();
    }
  }
  else
    addTail();

  return 1;
}

void LinkedList::addTail() {
  assert(rawptr.tail);
  sf::Vector2f pos = rawptr.tail->sprite.getPosition();
  pos.x += rawptr.tail->size.x + 10.f;
  std::unique_ptr<LLNode> newNode = std::make_unique<LLNode>(pos, LLText, nodeTexture);
  rawptr.tail->next = std::move(newNode);
  rawptr.tail = rawptr.tail->next.get();
  updatePrevPtr(rawptr.tail);
  rawptr.tail->updateLine(rawptr.prev);
  rawptr.tail->data += rawptr.prev->data;
  rawptr.tail->setText(rawptr.tail->data);
  if (rawptr.prev != rawptr.active && rawptr.prev != head.get())
    rawptr.prev->sprite.setColor(sf::Color::White);
  rawptr.tail->sprite.setColor(sf::Color(50, 140, 235));
}

bool LinkedList::shiftForward(LLNode* prev, LLNode* curr) {
  // ez solution is to just shift the newly inserted node forward until no intersection
  // harder and inefficient solution is to continually shift the nodes in front
  // of the newly inserted node until theres space for all nodes
  findAllNodeBounds(curr);
  sf::Vector2f pos = curr->sprite.getPosition();
  pos.x += curr->size.x + 10.f;
  for (const auto& anysprite : nBounds) {
    if (curr->sprite.getGlobalBounds().intersects(anysprite)) {
      curr->move(pos);
      curr->updateLine(prev);
      return false;
    }
  }
  return true;
}

bool LinkedList::remove() {
  // 0 nodes
  if (!head)
    return true;
  // 1 node
  else if (!head->next) {
    head = nullptr;
    rawptr.active = nullptr;
    rawptr.prev = nullptr;
    rawptr.currAnimate = nullptr;
    rawptr.tail = nullptr;
  }
  // 2+ nodes
  else if (rawptr.active) {
    if (rawptr.active == head.get()) {
      rawptr.active = rawptr.active->next.get();
      head = std::move(head->next);
      rawptr.active->sprite.setColor(sf::Color::Red);
      updatePrevPtr(rawptr.active);
    }
    else if (rawptr.active->next.get()) {
      updatePrevPtr(rawptr.active);
      rawptr.prev->next = std::move(rawptr.active->next);
      rawptr.prev->next.get()->updateLine(rawptr.prev);
      rawptr.active = nullptr;
      rawptr.prev = nullptr;
    }
    else if (!rawptr.active->next.get()) {
      updatePrevPtr(rawptr.active);
      rawptr.prev->next = nullptr;
      rawptr.prev->updateLine(nullptr);
      rawptr.active = rawptr.prev;
      updatePrevPtr(rawptr.active);
      rawptr.tail = rawptr.active;
      if (rawptr.tail != head.get())
        rawptr.tail->sprite.setColor(sf::Color(50, 140, 235));
    }
  }
  // default tail removal
  else {
    updatePrevPtr(rawptr.tail);
    if (rawptr.active == rawptr.tail)
      rawptr.active = nullptr;
    rawptr.prev->next = nullptr;
    rawptr.prev->updateLine(nullptr);
    rawptr.tail = rawptr.prev;
    if (rawptr.tail != head.get())
      rawptr.tail->sprite.setColor(sf::Color(50, 140, 235));
    rawptr.prev = nullptr;
  }

  return true;
}

// Notes:
// mapCoordsToPixel goes from world coordinates(game world, the relative view, objects in the view) to pixel coordinates(window / actual screen).
// mapPixelsToCoords goes from pixel coordinates(window / actual screen) to world coordinates(game world, relative view, objects in the view).
// Coordinates are relative (sfml getPositions() returns relative coords)
// Pixels are absolute
//
// I likely need to use mapPixelsToCoords because sprites are all drawptrn
// with relative positioning (coords). So I need to map my cursor's absolute
// position (pixels) to the relative pixels of the view (coords). Also, since
// I want to keep track of the cursor's positioning within the bounds of the
// sprite, I have to keep track of the relative sizing of the sprite as well,
// so converting my absolute mouse position to the relative position is the way to go.
// find the node the cursor clicks on based on coordinates
bool LinkedList::search(const sf::Vector2i mpos) {
  // TODO: more efficient is to just store node positions in a vector 
  // and search through that list. update the vector whenever the LL changes
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  if (!head)
    return false;

  // check if we clicked on the same active node
  if (rawptr.active) {
    sf::FloatRect activeBounds = rawptr.active->sprite.getGlobalBounds();
    if (activeBounds.contains(convertMPos.x, convertMPos.y))
      return true;
  }
  // otherwise, compare curr mpos with nodes or a spot on the screen
  for (LLNode* curr = head.get(); curr; curr = curr->next.get()) {
    sf::FloatRect currBounds = curr->sprite.getGlobalBounds();
    if (currBounds.contains(convertMPos.x, convertMPos.y)) {
      if (rawptr.active && rawptr.active != head.get() && rawptr.active != rawptr.tail)
        rawptr.active->sprite.setColor(sf::Color::White);
      rawptr.active = curr;
      if (rawptr.active != head.get() && rawptr.active != rawptr.tail)
        rawptr.active->sprite.setColor(sf::Color::Green);
      else if (rawptr.active == head.get() && head->sprite.getColor() == sf::Color::Magenta)
        rawptr.active->sprite.setColor(sf::Color::Red);
      else if (rawptr.active == rawptr.tail && rawptr.tail->sprite.getColor() == sf::Color::Magenta)
        rawptr.active->sprite.setColor(sf::Color(50, 140, 235));
      // update previous pointer after setting rawptr.active
      updatePrevPtr(rawptr.active);
      return true;
    }
  }
  return false;
}

int LinkedList::updatePrevPtr(LLNode* ptr) {
  if (!head || !head->next || !ptr || ptr == head.get()) {
    rawptr.prev = nullptr;
    return 0;
  }

  rawptr.prev = head.get();
  while (rawptr.prev->next.get() != ptr) {
    rawptr.prev = rawptr.prev->next.get();
  }

  return 1;
}

bool LinkedList::move(const sf::Vector2i mpos) {
  if (rawptr.active) {
    sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
    sf::Vector2f lastPos = rawptr.active->sprite.getPosition();

    rawptr.active->move(convertMPos); // moves sprite
    rawptr.active->updateLine(rawptr.prev); // update lines to follow sprite

    /*
    sf::FloatRect currsprite = rawptr.active->sprite.getGlobalBounds();
    findAllNodeBounds(rawptr.active);
    for (const auto& anysprite : nBounds) {
      if (currsprite.intersects(anysprite)) {
        //
        if (abs(velx) > abs(vely))
          rawptr.active->update(sf::Vector2f(lastPos.x, target.y+vely), rawptr.prev);
        else if (abs(velx) < abs(vely))
          rawptr.active->update(sf::Vector2f(target.x+velx, lastPos.y), rawptr.prev);
        else
        //
          rawptr.active->update(lastPos, rawptr.prev);
      }
    }
    */
    return true;
  }
  return false;
}

bool LinkedList::mouseInBounds(const sf::Vector2i mpos) const {
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  if (rawptr.active) {
    sf::FloatRect globalBounds = rawptr.active->sprite.getGlobalBounds();
    return globalBounds.contains(convertMPos.x, convertMPos.y);
  }
  return false;
}

bool LinkedList::findAllNodeBounds(LLNode* curr) {
  nBounds.clear();
  if (!head)
    return false;

  // exclude the compared node
  for (LLNode* temp = head.get(); temp; temp = temp->next.get()) {
    if (temp != curr)
      nBounds.emplace_back(temp->sprite.getGlobalBounds());
  }

  return true;
}

bool LinkedList::findValueAnimated(const int val) {
  // true means found or finished search space
  if (!head) {
    resetState();
    return true;
  }

  updateCursor(rawptr.currAnimate);
  switch (state) {
    case LLState::ENTRY: {
      rawptr.currAnimate = head.get();
      state = LLState::HIGHLIGHT;
      // TODO: need to restart clock on the first wait cycle
    } break;
    case LLState::HIGHLIGHT: {
      if (!rawptr.currAnimate) {
        resetState();
        return true;
      }
      
      if (rawptr.currAnimate != head.get() && rawptr.currAnimate != rawptr.tail && rawptr.currAnimate != rawptr.active)
          rawptr.currAnimate->sprite.setColor(sf::Color::Yellow);

      prevState = state;
      state = LLState::WAIT;

    } break;
    case LLState::WAIT: {
      // TODO: need this condition to only apply once, otherwise the animation speed gets wonky.
      if (cursorReached) {
        if (delayClock.getElapsedTime().asSeconds() >= DELAY) {
          if (prevState == LLState::HIGHLIGHT) {
            prevState = state;
            state = LLState::COMPARE;
          }
          else if (prevState == LLState::COMPARE) {
            prevState = state;
            state = LLState::HIGHLIGHT;
          }
        }
      }
    } break;
    case LLState::COMPARE: {
      if (rawptr.currAnimate->data == val) {
        rawptr.currAnimate->sprite.setColor(sf::Color::Magenta);
        resetState();
        return true;
      }
      else if (rawptr.currAnimate->next) {
        if (rawptr.currAnimate != head.get() && rawptr.currAnimate != rawptr.tail && rawptr.currAnimate != rawptr.active)
          rawptr.currAnimate->sprite.setColor(sf::Color::White);
        rawptr.currAnimate = rawptr.currAnimate->next.get();
      }
      else {
        if (rawptr.currAnimate != head.get() && rawptr.currAnimate != rawptr.tail && rawptr.currAnimate != rawptr.active)
          rawptr.currAnimate->sprite.setColor(sf::Color::White);
        resetState();
        return true;
      }
      prevState = state;
      state = LLState::WAIT;
    } break;
  }
  return false;
}

void LinkedList::dtRestart() { 
  if (state != LLState::WAIT || !cursorReached)
    delayClock.restart();
}

void LinkedList::updateCursor(LLNode* target) {
  sf::Clock clock;
  if (!target) {
    cursor.setRadius(0.f);
    return;
  }
  
  cursor.setRadius(10.f);
  cursor.setFillColor(target->sprite.getColor());

  sf::Vector2f currPos = cursor.getPosition();
  sf::Vector2f goal = target->sprite.getPosition();
  goal.y += (target->size.y * 0.5) + 20;
  float dx = goal.x - currPos.x;
  float dy = goal.y - currPos.y;
  float distance = float(sqrt(pow(dx, 2) + pow(dy, 2))); // euclidean distance formula
  if (distance >= 3) { 
    // calculate whats necessary
    float deltaTime = clock.restart().asSeconds();
    float k = 2000 * deltaTime; // dampening constant
    sf::Vector2f direction(dx / distance, dy / distance); // normalize dx,dy direction
    float velx = (k * distance * direction.x);
    float vely = (k * distance * direction.y);
    sf::Vector2f offset(velx, vely);
    cursor.move(offset);
    cursorReached = false;
  } 
  else {
    // TODO: idle animation
    cursor.setPosition(goal);
    cursorReached = true;
  }
}

// TODO: this is HELLA scuffed
// takes the linked list values in text form, finds which value is selected/highlighted
// and returns them to be handled with the imgui text to be colored
std::vector<std::string> LinkedList::parseString(LLNode* follow) {
  if (!head)
    return {};

  std::vector<int> data;
  int i = 0;
  int colorIndex = -1;
  LLNode* curr = head.get();

  for (LLNode* curr = head.get(); curr; curr = curr->next.get()) {
    data.emplace_back(curr->data);
    if (follow && curr == follow)
      colorIndex = i;
    ++i;
  }

  std::vector<std::string> text;
  if (colorIndex == -1) {
    // there's no colored index
    text.reserve(1);
    std::string str;
    for (int i = 0; i < data.size(); ++i) {
      str += std::to_string(data.at(i)) + "->";
    }
    text.emplace_back(str);
  }
  else if (colorIndex == 0 || colorIndex == data.size() - 1) {
    text.reserve(2);
    std::string colorStr;
    std::string str;
    // color at the ends
    // but which end... how do i differentiate which end when i return
    // SCUFFED
    if (colorIndex == 0) {
      colorStr += 'f';
      for (int i = 1; i < data.size(); ++i) {
        str += std::to_string(data.at(i)) + "->";
      }
    }
    else {
      colorStr += 'b';
      for (int i = 0; i < data.size() - 1; ++i) {
        str += std::to_string(data.at(i)) + "->";
      }
    }
    colorStr += (std::to_string(data.at(colorIndex)));
    text.emplace_back(colorStr);

    text.emplace_back(str);
  }
  else {
    // color is in the middle
    text.reserve(3);
    std::string leftStr;
    std::string colorStr(std::to_string(data.at(colorIndex)));
    std::string rightStr;
    for (int i = 0; i < colorIndex; ++i) {
      leftStr += std::to_string(data.at(i)) + "->";
    }
    for (int i = colorIndex + 1; i < data.size(); ++i) {
      rightStr += std::to_string(data.at(i)) + "->";
    }
    text.emplace_back(leftStr);
    text.emplace_back(colorStr);
    text.emplace_back(rightStr);
  }

  return text;
}

void LinkedList::resetState() {
  rawptr.currAnimate = nullptr;
  prevState = LLState::ENTRY;
  state = LLState::ENTRY;
}

bool LinkedList::resetActiveNode() {
  if (rawptr.active && rawptr.active != head.get() && rawptr.active != rawptr.tail)
    rawptr.active->sprite.setColor(sf::Color::White);
  rawptr.active = nullptr;
  return true;
}

int LinkedList::draw() const {
  if (!head)
    return 0;
  LLNode* curr = head.get();
  while (curr) {
    window.draw(*curr); // invokes overridden drawptr function in LLNode
    curr = curr->next.get();
  }
  window.draw(cursor);
  return 1;
}

int LinkedList::transformText(const std::vector<std::string>& text) {
  if (text.empty())
    return 0;

  std::string str;
  if (text.size() == 1)
    str += "head->" + text.at(0) + "null";
  else if (text.size() == 2) {
    // ">null" is on purpose because I need to account for the extra 'f'/'b' char
    str += "head->" + text.at(0) + text.at(1) + ">null";
  }
  else if (text.size() == 3)
    str += "head->" + text.at(0) + text.at(1) + text.at(2) + "->null";
  else
    throw std::invalid_argument("vector contains wrong values");
   
  ImVec2 availSpace = ImGui::GetContentRegionAvail();
  ImVec2 textSize = ImGui::CalcTextSize(str.c_str());

  // scale text
  float scaleAmount = 0.1f;
  if (prevTextSize != textSize.x && availSpace.x / textSize.x < 1.f) {
    prevTextSize = textSize.x;
    textScale -= scaleAmount;
  }
  else if (
    prevTextSize != textSize.x && 
    availSpace.x / (textSize.x * (1 + scaleAmount)) > 1.f && 
    textScale < 5
    ) {
    prevTextSize = textSize.x;
    textScale += scaleAmount;
  }

  ImVec2 padding(0, 0);
  padding.x = (availSpace.x - textSize.x) / 2;
  padding.y = (availSpace.y - textSize.y) / 2;
  ImGui::SetWindowFontScale(textScale);
  ImGui::SetCursorPos(padding);
  return 1;
}
