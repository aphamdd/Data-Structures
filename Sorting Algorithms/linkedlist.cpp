#pragma once
#include "LinkedList.h"

// TODO: is there a way to skip having to pass in my text through LinkedList->LLNode
LinkedList::LinkedList(sf::RenderWindow& win, sf::Text& text) :
  window(win),
  LLText(text),
  cursor(0.f, 3) {
  cursor.setFillColor(sf::Color::Green);
  cursor.setOutlineThickness(1.5f);
  cursor.setOutlineColor(sf::Color::Black);
}
LinkedList::~LinkedList() {
  clear();
}

void LinkedList::clear() {
  LLNode* temp = mHead;
  while (mHead) {
    mHead = mHead->next;
    delete temp;
    temp = mHead;
  }
  temp = nullptr;
  mHead = nullptr;
  mActive = nullptr;
  mPrev = nullptr;
  mStatePtr = nullptr;
}

void LinkedList::add() {
  sf::Vector2f tempPos(75.f, 400.f);
  if (!mHead) {
    mHead = new LLNode(tempPos, LLText);
    mHead->shape.setFillColor(sf::Color::Red);
  }
  else {
    LLNode* curr = mHead;
    while (curr->next) {
      curr = curr->next;
    }
    sf::Vector2f temp = curr->shape.getPosition();
    temp.x += 110.f;
    curr->next = new LLNode(temp, LLText);
    curr->next->ID += curr->ID;
    curr->next->updateNext(curr);
    // TODO: create a function to update any changes to text/shape
    curr->next->dataText.setString(std::to_string(curr->next->ID));
  }
}

bool LinkedList::remove() {
  // 0 nodes
  if (!mHead) {
    return true;
  }
  // 1 node
  else if (!mHead->next) {
    delete mHead;
    mHead = nullptr;
    mActive = nullptr;
    mPrev = nullptr;
    mStatePtr = nullptr;
    return true;
  }
  // 2+ nodes
  LLNode* prev = nullptr;
  LLNode* curr = mHead;
  while (curr->next) {
    prev = curr;
    curr = curr->next;
  }
  if (mPrev == curr)
    mPrev = nullptr;
  if (mActive == curr)
    mActive = nullptr;
  delete curr;
  curr = nullptr;
  prev->next = nullptr;
  prev->updateNext();
  prev = nullptr;

  return true;
}

// find the node the cursor clicks on based on coordinates
LLNode* LinkedList::search(const sf::Vector2i mpos) {
  // mapCoordsToPixel goes from world coordinates(game world, the relative view, objects in the view) to pixel coordinates(window / actual screen).
  // mapPixelsToCoords goes from pixel coordinates(window / actual screen) to world coordinates(game world, relative view, objects in the view).
  // Coordinates are relative (sfml getPositions() returns relative coords)
  // Pixels are absolute
  //
  // I likely need to use mapPixelsToCoords because shapes are all drawn
  // with relative positioning (coords). So I need to map my cursor's absolute
  // position (pixels) to the relative pixels of the view (coords). Also, since
  // I want to keep track of the cursor's positioning within the bounds of the
  // shape, I have to keep track of the relative sizing of the shape as well,
  // so converting my absolute mouse position to the relative position is the way to go.
  
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  if (!mHead)
    return nullptr;

  // check if we clicked on the same active node
  if (mActive) {
    sf::FloatRect activeBounds = mActive->shape.getGlobalBounds();
    if (activeBounds.contains(convertMPos.x, convertMPos.y)) {
      return mActive;
    }
  }
  // otherwise, compare curr mpos with nodes or a spot on the screen
  for (LLNode* curr = mHead; curr; curr = curr->next) {
    sf::FloatRect currBounds = curr->shape.getGlobalBounds();
    if (currBounds.contains(convertMPos.x, convertMPos.y)) {
      // set pactive color to white, set new pactive, highlight new pactive
      if (mActive)
        mActive->shape.setFillColor(sf::Color::White);
      mActive = curr;
      mActive->shape.setFillColor(sf::Color::Green);
      // update previous pointer after setting mActive
      updatePrev();
      return mActive;
    }
  }
  return nullptr;
}

void LinkedList::updatePrev() {
  if (!mHead || !mHead->next || !mActive || mActive == mHead) {
    mPrev = nullptr;
    return;
  }
  mPrev = mHead;
  while (mPrev->next != mActive) {
    mPrev = mPrev->next;
  }
}

bool LinkedList::move(const sf::Vector2i mpos) {
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  sf::Vector2f lastPos = mActive->shape.getPosition();
  if (mActive) {
    mActive->update(convertMPos, mPrev);
    // handle collision
    sf::FloatRect currShape = mActive->shape.getGlobalBounds();
    findNodeBounds();
    for (const auto& anyShape : nBounds) {
      if (currShape.intersects(anyShape))
        mActive->update(lastPos, mPrev);
    }
    return true;
  }
  return false;
}

bool LinkedList::isInBounds(const sf::Vector2i mpos) const {
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  if (mActive) {
    sf::FloatRect globalBounds = mActive->shape.getGlobalBounds();
    return globalBounds.contains(convertMPos.x, convertMPos.y);
  }
  return false;
}

bool LinkedList::findNodeBounds() {
  nBounds.clear();
  if (!mHead)
    return false;

  LLNode* curr = mHead;
  while (curr) {
    // don't insert the active node 
    if (curr != mActive)
      nBounds.push_back(curr->shape.getGlobalBounds());
    curr = curr->next;
  }
  return true;
}

bool LinkedList::findValue(const int val) {
  // true means found or finished search space
  if (!mHead) {
    resetState();
    return true;
  }

  switch (state) {
    case LLState::ENTRY: {
      mStatePtr = mHead;
      state = LLState::COMPARE;
      delayClock.restart();
    } break;
    case LLState::HIGHLIGHT: {
      if (mStatePtr) {
        mStatePtr->shape.setFillColor(sf::Color::Yellow);
      }
      else {
        resetState();
        return true;
      }
      prevState = state;
      state = LLState::WAIT;
      delayClock.restart();
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
      if (mStatePtr->ID == val) {
        mStatePtr->shape.setFillColor(sf::Color::Magenta);
        resetState();
        std::cout << "found " << val << std::endl;
        return true;
      }
      if (mStatePtr->next) {
        mStatePtr->shape.setFillColor(sf::Color::White);
        mStatePtr = mStatePtr->next;
      }
      else {
        mStatePtr->shape.setFillColor(sf::Color::White);
        resetState();
        return true;
      }
      prevState = state;
      state = LLState::WAIT;
      delayClock.restart();
    } break;
  }
  return false;
}

void LinkedList::updateCursor() {
  // doesn't ever get called cuz updateCursor() only gets called when moved
  if (!mActive) {
    cursor.setRadius(0.f);
    cursor.setPosition(sf::Vector2f(0, 0));
    mLastActive = nullptr;
    return;
  }
  // handles first time mActive points to a node
  // TODO: change mLastActive to something else
  else if (mActive && !mLastActive) {
    mLastActive = mActive;
    sf::Vector2f activeNodePos = mActive->shape.getPosition();
    activeNodePos.y += mActive->shape.getSize().y;
    cursor.setRadius(10.f);
    cursor.setPosition(activeNodePos);
  }

  // TODO: refactor for efficiency
  float k = 10000; // dampening constant
  sf::Vector2f cursorPos = cursor.getPosition();
  sf::Vector2f cursorGoal = mActive->shape.getPosition();
  cursorGoal.y += mActive->shape.getSize().y;
  float dx = cursorGoal.x - cursorPos.x;
  float dy = cursorGoal.y - cursorPos.y;
  float distance = sqrt(pow(dx, 2) + pow(dy, 2)); // euclidean distance formula
  sf::Vector2f direction(dx / distance, dy / distance); // normalize dx,dy direction
  float dt = delayClock.restart().asSeconds();
  float velx = (k * distance * direction.x)*dt;
  float vely = (k * distance * direction.y)*dt;
  // not 0 because itll never reach 0, just needs to be "close enough"
  if (distance >= 5) { 
    sf::Vector2f res(cursorPos.x + velx, cursorPos.y + vely);
    cursor.setPosition(res);
  }
  else {
    cursor.setPosition(cursorGoal);
    mLastActive = mActive;
  }
}

void LinkedList::resetState() {
  mStatePtr = nullptr;
  state = LLState::ENTRY;
  prevState = state;
}

void LinkedList::draw() const {
  if (!mHead)
    return;
  LLNode* curr = mHead;
  while (curr) {
    window.draw(*curr); // invokes overridden draw function in LLNode
    curr = curr->next;
  }
  window.draw(cursor);
}