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
    if (!texture.loadFromFile("./Textures/testframe.png"))
      throw std::runtime_error("texture load error");
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
LinkedList::~LinkedList() {
  clear();
}

void LinkedList::clear() {
  LLNode* temp = LLPtr.head;
  while (LLPtr.head) {
    LLPtr.head = LLPtr.head->next;
    delete temp;
    temp = LLPtr.head;
  }
  temp = nullptr;
  mActive = nullptr;
  mStatePtr = nullptr;
  LLPtr.head = nullptr;
  LLPtr.prev = nullptr;
  LLPtr.tail = nullptr;
}

// TODO: don't return midway of a statement, return at the end
// so I can shift any intersecting node
int LinkedList::add() {
  // create head
  if (!LLPtr.head) {
    sf::Vector2f pos(75.f, 400.f);
    LLPtr.head = new LLNode(pos, LLText, texture);
    LLPtr.head->sprite.setColor(sf::Color::Red);
    LLPtr.tail = LLPtr.head;
  }
  // insert in front of active ptr
  else if (mActive && mActive->next) {
    LLNode* temp = mActive->next;
    sf::Vector2f pos = mActive->sprite.getPosition();
    pos.x += mActive->size.x + 10.f;
    mActive->next = new LLNode(pos, LLText, texture);
    if (mActive != LLPtr.head)
      mActive->sprite.setColor(sf::Color::White);
    mActive = mActive->next;
    updatePrevPtr(mActive);
    if (mActive != LLPtr.tail)
      mActive->sprite.setColor(sf::Color::Green);
    mActive->next = temp;
    mActive->data += LLPtr.prev->data;
    mActive->updateLine(LLPtr.prev);
    mActive->next->updateLine(mActive);
    mActive->setText(mActive->data);
    // TODO: I don't like having a loop like this, fix it. recursion?
    // bug: only if mActive->next, needs to apply when ANY node is added
    while (!shiftForward(mActive, mActive->next)) {
      continue;
    }
    temp = nullptr;
  }
  else {
    assert(LLPtr.tail);
    LLNode* curr = LLPtr.tail;
    sf::Vector2f pos = curr->sprite.getPosition();
    pos.x += curr->size.x + 10.f;
    curr->next = new LLNode(pos, LLText, texture);
    curr->next->data += curr->data;
    curr->next->updateLine(curr);
    curr->next->setText(curr->next->data);
    if (LLPtr.tail != mActive && LLPtr.tail != LLPtr.head)
      LLPtr.tail->sprite.setColor(sf::Color::White);
    LLPtr.tail = curr->next;
    LLPtr.tail->sprite.setColor(sf::Color(50, 140, 235));
    curr = nullptr;
  }
  return 1;
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
  if (!LLPtr.head)
    return true;
  // 1 node
  else if (!LLPtr.head->next) {
    delete LLPtr.head;
    LLPtr.head = nullptr;
    mActive = nullptr;
    LLPtr.prev = nullptr;
    mStatePtr = nullptr;
    LLPtr.tail = nullptr;
  }
  // 2+ nodes
  else if (mActive) {
    if (mActive == LLPtr.head) {
      mActive = mActive->next;
      mActive->sprite.setColor(sf::Color::Red);
      delete LLPtr.head;
      LLPtr.head = mActive;
    }
    else if (mActive->next) {
      updatePrevPtr(mActive);
      LLNode* temp = mActive->next;
      delete mActive;
      LLPtr.prev->next = temp;
      temp->updateLine(LLPtr.prev);
      mActive = nullptr;
      LLPtr.prev = nullptr;
      temp = nullptr;
    }
  }
  // default tail removal
  else {
    updatePrevPtr(LLPtr.tail);
    LLNode* curr = LLPtr.tail;
    if (mActive == curr)
      mActive = nullptr;
    delete curr;
    curr = nullptr;
    LLPtr.prev->next = nullptr;
    LLPtr.prev->updateLine(nullptr);
    LLPtr.tail = LLPtr.prev;
    LLPtr.tail->sprite.setColor(sf::Color(50, 140, 235));
  }

  return true;
}

// find the node the cursor clicks on based on coordinates
LLNode* LinkedList::search(const sf::Vector2i mpos) {
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
  
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  if (!LLPtr.head)
    return nullptr;

  // check if we clicked on the same active node
  if (mActive) {
    sf::FloatRect activeBounds = mActive->sprite.getGlobalBounds();
    if (activeBounds.contains(convertMPos.x, convertMPos.y))
      return mActive;
  }
  // otherwise, compare curr mpos with nodes or a spot on the screen
  for (LLNode* curr = LLPtr.head; curr; curr = curr->next) {
    sf::FloatRect currBounds = curr->sprite.getGlobalBounds();
    if (currBounds.contains(convertMPos.x, convertMPos.y)) {
      if (mActive && mActive != LLPtr.head && mActive != LLPtr.tail)
        mActive->sprite.setColor(sf::Color::White);
      mActive = curr;
      if (mActive != LLPtr.head && mActive != LLPtr.tail)
        mActive->sprite.setColor(sf::Color::Green);
      // update previous pointer after setting mActive
      updatePrevPtr(mActive);
      return mActive;
    }
  }
  return nullptr;
}

int LinkedList::updatePrevPtr(LLNode* ptr) {
  if (!LLPtr.head || !LLPtr.head->next || !ptr || ptr == LLPtr.head) {
    LLPtr.prev = nullptr;
    return 0;
  }

  LLPtr.prev = LLPtr.head;
  while (LLPtr.prev->next != ptr) {
    LLPtr.prev = LLPtr.prev->next;
  }

  return 1;
}

bool LinkedList::move(const sf::Vector2i mpos) {
  sf::Clock clock;
  if (mActive) {
    sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
    /*
    sf::Vector2f lastPos = mActive->sprite.getPosition();
    float k = 500;
    sf::Vector2f target = mActive->sprite.getPosition();
    sf::Vector2f goal = convertMPos;
    float dx = goal.x - target.x;
    float dy = goal.y - target.y;
    float distance = sqrt(pow(dx, 2) + pow(dy, 2)); // euclidean distance formula
    sf::Vector2f direction(dx, dy);
    float deltaTime = clock.restart().asSeconds();
    float velx = (k * distance * direction.x) * deltaTime;
    float vely = (k * distance * direction.y) * deltaTime;
    sf::Vector2f res(target.x + velx, target.y + vely);
    mActive->update(res, LLPtr.prev);
    */
    mActive->move(convertMPos); // moves sprite
    mActive->updateLine(LLPtr.prev); // update lines to follow sprite

    /*
    sf::FloatRect currsprite = mActive->sprite.getGlobalBounds();
    findAllNodeBounds(mActive);
    for (const auto& anysprite : nBounds) {
      if (currsprite.intersects(anysprite)) {
        //
        if (abs(velx) > abs(vely))
          mActive->update(sf::Vector2f(lastPos.x, target.y+vely), LLPtr.prev);
        else if (abs(velx) < abs(vely))
          mActive->update(sf::Vector2f(target.x+velx, lastPos.y), LLPtr.prev);
        else
        //
          mActive->update(lastPos, LLPtr.prev);
      }
    }
    */
    return true;
  }
  return false;
}

bool LinkedList::mouseInBounds(const sf::Vector2i mpos) const {
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  if (mActive) {
    sf::FloatRect globalBounds = mActive->sprite.getGlobalBounds();
    return globalBounds.contains(convertMPos.x, convertMPos.y);
  }
  return false;
}

bool LinkedList::findAllNodeBounds(LLNode* curr) {
  nBounds.clear();
  if (!LLPtr.head)
    return false;

  // exclude the compared node
  for (LLNode* temp = LLPtr.head; temp; temp = temp->next) {
    if (temp != curr)
      nBounds.push_back(temp->sprite.getGlobalBounds());
  }

  return true;
}

bool LinkedList::findValueAnimated(const int val) {
  // true means found or finished search space
  if (!LLPtr.head) {
    resetState();
    return true;
  }

  updateCursor(mStatePtr);
  switch (state) {
    case LLState::ENTRY: {
      mStatePtr = LLPtr.head;
      state = LLState::HIGHLIGHT;
    } break;
    case LLState::HIGHLIGHT: {
      if (!mStatePtr) {
        resetState();
        return true;
      }
      
      if (mStatePtr != LLPtr.head && mStatePtr != LLPtr.tail && mStatePtr != mActive)
          mStatePtr->sprite.setColor(sf::Color::Yellow);
      prevState = state;
      state = LLState::WAIT;
    } break;
    case LLState::WAIT: {
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
    } break;
    case LLState::COMPARE: {
      if (mStatePtr->data == val) {
        mStatePtr->sprite.setColor(sf::Color::Magenta);
        resetState();
        return true;
      }
      else if (mStatePtr->next) {
        if (mStatePtr != LLPtr.head && mStatePtr != LLPtr.tail && mStatePtr != mActive)
          mStatePtr->sprite.setColor(sf::Color::White);
        mStatePtr = mStatePtr->next;
      }
      else {
        if (mStatePtr != LLPtr.head && mStatePtr != LLPtr.tail && mStatePtr != mActive)
          mStatePtr->sprite.setColor(sf::Color::White);
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
  if (state != LLState::WAIT)
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
  goal.y += target->size.y;
  float dx = goal.x - currPos.x;
  float dy = goal.y - currPos.y;
  float distance = sqrt(pow(dx, 2) + pow(dy, 2)); // euclidean distance formula
  float deltaTime = clock.restart().asSeconds();
  // not 0 because itll never reach 0, just needs to be "close enough"
  if (distance >= 3) { 
    // calculate whats necessary
    float k = 1800; // dampening constant
    sf::Vector2f direction(dx / distance, dy / distance); // normalize dx,dy direction
    float velx = (k * distance * direction.x) * deltaTime;
    float vely = (k * distance * direction.y) * deltaTime;
    cursor.move(sf::Vector2f(velx, vely));
  } 
  else {
    // TODO: idle animation
    cursor.setPosition(goal);
  }
}

// TODO: this is HELLA scuffed
// takes the linked list values in text form, finds which value is selected/highlighted
// and returns them to be handled with the imgui text to be colored
std::vector<std::string> LinkedList::parseString(LLNode* follow) {
  if (!LLPtr.head)
    return {};

  std::vector<int> data;
  int i = 0;
  int colorIndex = -1;
  LLNode* curr = LLPtr.head;

  for (LLNode* curr = LLPtr.head; curr; curr = curr->next) {
    data.push_back(curr->data);
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
    text.push_back(str);
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
    text.push_back(colorStr);

    text.push_back(str);
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
    text.push_back(leftStr);
    text.push_back(colorStr);
    text.push_back(rightStr);
  }

  return text;
}

void LinkedList::resetState() {
  mStatePtr = nullptr;
  prevState = LLState::ENTRY;
  state = LLState::ENTRY;
}

bool LinkedList::resetActiveNode() {
  if (mActive && mActive != LLPtr.head && mActive != LLPtr.tail)
    mActive->sprite.setColor(sf::Color::White);
  mActive = nullptr;
  return true;
}

int LinkedList::draw() const {
  if (!LLPtr.head)
    return 0;
  LLNode* curr = LLPtr.head;
  while (curr) {
    window.draw(*curr); // invokes overridden draw function in LLNode
    curr = curr->next;
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
  float scaleAmount = 0.1;
  if (prevTextSize != textSize.x && availSpace.x / textSize.x < 1.f) {
    prevTextSize = textSize.x;
    textScale -= scaleAmount;
  }
  else if (prevTextSize != textSize.x && availSpace.x / (textSize.x * (1 + scaleAmount)) > 1.f && textScale < 6) {
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
