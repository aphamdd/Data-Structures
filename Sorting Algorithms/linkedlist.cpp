#pragma once
#include "LinkedList.h"

// TODO: is there a way to skip having to pass in my text through LinkedList->LLNode
LinkedList::LinkedList(sf::RenderWindow& win, sf::Text& text) :
  window(win),
  LLText(text),
  nBounds(),
  mHead(NULL), 
  mActive(NULL), 
  mPrev(NULL),
  state(LLState::ENTRY),
  prevState(LLState::ENTRY),
  mStatePtr(NULL) {
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
  temp = NULL;
  mHead = NULL;
  mActive = NULL;
  mPrev = NULL;
  mStatePtr = NULL;
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
    mHead = NULL;
    mActive = NULL;
    mPrev = NULL;
    mStatePtr = NULL;
    return true;
  }
  // 2+ nodes
  LLNode* prev = NULL;
  LLNode* curr = mHead;
  while (curr->next) {
    prev = curr;
    curr = curr->next;
  }
  if (mPrev == curr)
    mPrev = NULL;
  if (mActive == curr)
    mActive = NULL;
  delete curr;
  curr = NULL;
  prev->next = NULL;
  prev->updateNext();
  prev = NULL;

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
    return NULL;

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
  return NULL;
}

void LinkedList::updatePrev() {
  if (!mHead || !mHead->next || !mActive || mActive == mHead) {
    mPrev = NULL;
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

void LinkedList::draw() const {
  if (!mHead)
    return;
  LLNode* curr = mHead;
  while (curr) {
    window.draw(*curr); // invokes overridden draw function in LLNode
    curr = curr->next;
  }
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

void LinkedList::resetState() {
  mStatePtr = NULL;
  state = LLState::ENTRY;
  prevState = state;
}