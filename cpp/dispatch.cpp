// https://en.wikipedia.org/wiki/Double_dispatch

#include <iostream>

class SpaceShip;
class ApolloSpacecraft;
class Asteroid {
public:
  virtual void CollideWith(SpaceShip &);
  virtual void CollideWith(ApolloSpacecraft &);
};

class SpaceShip {
public:
  virtual void CollideWith(Asteroid &inAsteroid) {
    inAsteroid.CollideWith(*this);
  }
};

class ApolloSpacecraft : public SpaceShip {
public:
  virtual void CollideWith(Asteroid &inAsteroid) override {
    inAsteroid.CollideWith(*this);
  }
};

void Asteroid::CollideWith(SpaceShip &) {
  std::cout << "Asteroid hit a SpaceShip" << std::endl;
}

void Asteroid::CollideWith(ApolloSpacecraft &) {
  std::cout << "Asteroid hit an ApolloSpacecraft" << std::endl;
}

class ExplodingAsteroid final : public Asteroid {
public:
  virtual void CollideWith(SpaceShip &) override {
    std::cout << "ExplodingAsteroid hit a SpaceShip" << std::endl;
  }
  virtual void CollideWith(ApolloSpacecraft &) override {
    std::cout << "ExplodingAsteroid hit an ApolloSpacecraft" << std::endl;
  }
};

int main(int argc, char *argv[]) {
  Asteroid theAsteroid;
  SpaceShip theSpaceShip;
  ApolloSpacecraft theApolloSpacecraft;

  // Function overloading.
  theAsteroid.CollideWith(theSpaceShip);
  theAsteroid.CollideWith(theApolloSpacecraft);

  std::cout << std::endl;

  // Function overloading.
  ExplodingAsteroid theExplodingAsteroid;
  theExplodingAsteroid.CollideWith(theSpaceShip);
  theExplodingAsteroid.CollideWith(theApolloSpacecraft);

  std::cout << std::endl;

  // Dynamic dispatch.
  Asteroid &theAsteroidReference = theExplodingAsteroid;
  theAsteroidReference.CollideWith(theSpaceShip);
  theAsteroidReference.CollideWith(theApolloSpacecraft);

  std::cout << std::endl;

  // Dynamic dispatch, does not work as desired.
  // The problem is that, while virtual functions are dispatched dynamically in
  // C++, function overloading is done statically.
  SpaceShip &theSpaceShipReference = theApolloSpacecraft;
  theAsteroid.CollideWith(theSpaceShipReference);
  theAsteroidReference.CollideWith(theSpaceShipReference);

  std::cout << std::endl;

  // Double dispatch, visitor pattern.
  theSpaceShipReference.CollideWith(theAsteroid);
  theSpaceShipReference.CollideWith(theAsteroidReference);

  return 0;
}
