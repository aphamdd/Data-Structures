#pragma once
#include "LinkedList.h"
#include <iostream>

// TODO: is there a way to skip having to pass in my text through LinkedList->LLNode
LinkedList::LinkedList(sf::RenderWindow& win) :
  window(win),
  cursor(0.f, 3) {
  cursor.setFillColor(sf::Color::White);
  cursor.setOutlineThickness(1.5f);
  cursor.setOutlineColor(sf::Color::Black);

  try {
    if (!GLOBAL::LLTEXTURE.loadFromFile("./Textures/linkedlistframePepe.png"))
      throw std::runtime_error("linked list node texture error");
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void LinkedList::clear() {
  while (head)
    head = std::move(head->next);
  raw.active = nullptr;
  raw.currAnimate = nullptr;
  head = nullptr;
  raw.prev = nullptr;
  raw.tail = nullptr;
}

// TODO: don't return midway of a statement, return at the end
// so I can shift any intersecting node
int LinkedList::add() {
  // create head
  if (!head) {
    sf::Vector2f pos(75.f, 400.f); // random ahh position
    head = std::make_unique<LLNode>(pos);
    head->sprite.setColor(sf::Color::Red);
    raw.tail = head.get();
  }
  // insert in front of active ptr
  else if (raw.active) {
    if (raw.active->next) {
      sf::Vector2f pos = raw.active->sprite.getPosition();
      pos.x += raw.active->size.x + 10.f;
      std::unique_ptr<LLNode> newNode = std::make_unique<LLNode>(pos);
      newNode->next = std::move(raw.active->next);
      raw.active->next = std::move(newNode);
      if (raw.active != head.get())
        raw.active->sprite.setColor(sf::Color::White);
      raw.active = raw.active->next.get();
      updatePrevPtr(raw.active);
      if (raw.active != raw.tail)
        raw.active->sprite.setColor(sf::Color::Green);
      raw.active->data += raw.prev->data;
      raw.active->updateLine(raw.prev);
      raw.active->next.get()->updateLine(raw.active);
      raw.active->setText(raw.active->data);
      // TODO: I don't like having a loop like this, fix it. recursion?
      // bug: only if raw.active->next, needs to apply when ANY node is added
      //while (!shiftForward(raw.active, raw.active->next.get())) {
        //continue;
      //}
    }
    else if (!raw.active->next) {
      addTail();
      if (raw.active != head.get())
        raw.active->sprite.setColor(sf::Color::White);
      raw.active = raw.active->next.get();
    }
  }
  else
    addTail();

  return 1;
}

void LinkedList::addTail() {
  assert(raw.tail);
  sf::Vector2f pos = raw.tail->sprite.getPosition();
  pos.x += raw.tail->size.x + 10.f;
  std::unique_ptr<LLNode> newNode = std::make_unique<LLNode>(pos);
  raw.tail->next = std::move(newNode);
  raw.tail = raw.tail->next.get();
  updatePrevPtr(raw.tail);
  raw.tail->updateLine(raw.prev);
  raw.tail->data += raw.prev->data;
  raw.tail->setText(raw.tail->data);
  if (raw.prev != raw.active && raw.prev != head.get())
    raw.prev->sprite.setColor(sf::Color::White);
  raw.tail->sprite.setColor(sf::Color(50, 140, 235));
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
    raw.active = nullptr;
    raw.prev = nullptr;
    raw.currAnimate = nullptr;
    raw.tail = nullptr;
  }
  // 2+ nodes
  else if (raw.active) {
    if (raw.active == head.get()) {
      raw.active = raw.active->next.get();
      head = std::move(head->next);
      raw.active->sprite.setColor(sf::Color::Red);
      updatePrevPtr(raw.active);
    }
    else if (raw.active->next.get()) {
      updatePrevPtr(raw.active);
      raw.prev->next = std::move(raw.active->next);
      raw.prev->next.get()->updateLine(raw.prev);
      raw.active = nullptr;
      raw.prev = nullptr;
    }
    else if (!raw.active->next.get()) {
      updatePrevPtr(raw.active);
      raw.prev->next = nullptr;
      raw.prev->updateLine(nullptr);
      raw.active = raw.prev;
      updatePrevPtr(raw.active);
      raw.tail = raw.active;
      if (raw.tail != head.get())
        raw.tail->sprite.setColor(sf::Color(50, 140, 235));
    }
  }
  // default tail removal
  else {
    updatePrevPtr(raw.tail);
    if (raw.active == raw.tail)
      raw.active = nullptr;
    raw.prev->next = nullptr;
    raw.prev->updateLine(nullptr);
    raw.tail = raw.prev;
    if (raw.tail != head.get())
      raw.tail->sprite.setColor(sf::Color(50, 140, 235));
    raw.prev = nullptr;
  }

  return true;
}

// Notes:
// mapCoordsToPixel goes from world coordinates(game world, the relative view, objects in the view) to pixel coordinates(window / actual screen).
// mapPixelsToCoords goes from pixel coordinates(window / actual screen) to world coordinates(game world, relative view, objects in the view).
// Coordinates are relative (sfml getPositions() returns relative coords)
// Pixels are absolute
//
// I likely need to use mapPixelsToCoords because sprites are all drawn
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
  if (raw.active) {
    sf::FloatRect activeBounds = raw.active->sprite.getGlobalBounds();
    if (activeBounds.contains(convertMPos.x, convertMPos.y))
      return true;
  }
  // otherwise, compare curr mpos with nodes or a spot on the screen
  for (LLNode* curr = head.get(); curr; curr = curr->next.get()) {
    sf::FloatRect currBounds = curr->sprite.getGlobalBounds();
    if (currBounds.contains(convertMPos.x, convertMPos.y)) {
      if (raw.active && raw.active != head.get() && raw.active != raw.tail)
        raw.active->sprite.setColor(sf::Color::White);
      raw.active = curr;
      if (raw.active != head.get() && raw.active != raw.tail)
        raw.active->sprite.setColor(sf::Color::Green);
      else if (raw.active == head.get() && head->sprite.getColor() == sf::Color::Magenta)
        raw.active->sprite.setColor(sf::Color::Red);
      else if (raw.active == raw.tail && raw.tail->sprite.getColor() == sf::Color::Magenta)
        raw.active->sprite.setColor(sf::Color(50, 140, 235));
      // update previous pointer after setting raw.active
      updatePrevPtr(raw.active);
      return true;
    }
  }
  return false;
}

int LinkedList::updatePrevPtr(LLNode* ptr) {
  if (!head || !head->next || !ptr || ptr == head.get()) {
    raw.prev = nullptr;
    return 0;
  }

  raw.prev = head.get();
  while (raw.prev->next.get() != ptr) {
    raw.prev = raw.prev->next.get();
  }

  return 1;
}

bool LinkedList::move(const sf::Vector2i mpos) {
  if (raw.active) {
    sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
    sf::Vector2f lastPos = raw.active->sprite.getPosition();

    raw.active->move(convertMPos); // moves sprite
    raw.active->updateLine(raw.prev); // update lines to follow sprite

    /*
    sf::FloatRect currsprite = raw.active->sprite.getGlobalBounds();
    findAllNodeBounds(raw.active);
    for (const auto& anysprite : nBounds) {
      if (currsprite.intersects(anysprite)) {
        //
        if (abs(velx) > abs(vely))
          raw.active->update(sf::Vector2f(lastPos.x, target.y+vely), raw.prev);
        else if (abs(velx) < abs(vely))
          raw.active->update(sf::Vector2f(target.x+velx, lastPos.y), raw.prev);
        else
        //
          raw.active->update(lastPos, raw.prev);
      }
    }
    */
    return true;
  }
  return false;
}

bool LinkedList::mouseInBounds(const sf::Vector2i mpos) const {
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  if (raw.active) {
    sf::FloatRect globalBounds = raw.active->sprite.getGlobalBounds();
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

  updateCursor(raw.currAnimate);
  switch (state) {
    case LLState::ENTRY: {
      raw.currAnimate = head.get();
      state = LLState::HIGHLIGHT;
      // TODO: need to restart clock on the first wait cycle
    } break;
    case LLState::HIGHLIGHT: {
      if (!raw.currAnimate) {
        resetState();
        return true;
      }
      
      if (raw.currAnimate != head.get() && raw.currAnimate != raw.tail && raw.currAnimate != raw.active)
          raw.currAnimate->sprite.setColor(sf::Color::Yellow);

      prevState = state;
      state = LLState::WAIT;

    } break;
    case LLState::WAIT: {
      // TODO: need this condition to only apply once, otherwise the animation speed gets wonky.
      if (cursorReached) {
        if (delayClock.getElapsedTime().asSeconds() >= GLOBAL::DELAY) {
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
      if (raw.currAnimate->data == val) {
        raw.currAnimate->sprite.setColor(sf::Color::Magenta);
        resetState();
        return true;
      }
      else if (raw.currAnimate->next) {
        if (raw.currAnimate != head.get() && raw.currAnimate != raw.tail && raw.currAnimate != raw.active)
          raw.currAnimate->sprite.setColor(sf::Color::White);
        raw.currAnimate = raw.currAnimate->next.get();
      }
      else {
        if (raw.currAnimate != head.get() && raw.currAnimate != raw.tail && raw.currAnimate != raw.active)
          raw.currAnimate->sprite.setColor(sf::Color::White);
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
  raw.currAnimate = nullptr;
  prevState = LLState::ENTRY;
  state = LLState::ENTRY;
}

bool LinkedList::resetActiveNode() {
  if (raw.active && raw.active != head.get() && raw.active != raw.tail)
    raw.active->sprite.setColor(sf::Color::White);
  raw.active = nullptr;
  return true;
}

int LinkedList::draw() const {
  if (!head)
    return 0;
  LLNode* curr = head.get();
  while (curr) {
    window.draw(*curr); // invokes overridden draw function in LLNode
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
