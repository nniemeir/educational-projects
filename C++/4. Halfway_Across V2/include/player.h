#ifndef PLAYER_H_
#define PLAYER_H_
#include "halfway.h"

class playerStats {
public:
  playerStats();

  // Stats
  int getHealth() const;
  void setHealth(int health);
  int getEnergy() const;
  void setEnergy(int energy);
  int getHunger() const;
  void setHunger(int hunger);
  int getThirst() const;
  void setThirst(int thirst);
  int getWarmth() const;
  void setWarmth(int warmth);
  int getCharm() const;
  void setCharm(int charm);
  int getStanding() const;
  void setStanding(int standing);
  int constrainStat(int stat);
  QString constructReflection() const;

  // Inventory
  int searchInventory(const QString &itemName) const;
  QString clothesInventory();
  QString bagInventory();
  const item &getInventoryItem(int index) const;
  void addItem(item itemToAdd);
  void removeItem(int index);
  QString getItemName(int index) const;
  int getItemEffect(int index) const;
  int getItemEquipped(int index) const;
  void setItemEquipped(int index, int value);
  int getItemQuantity(int index) const;
  void setItemQuantity(int index, int value);

private:
  int health;
  int energy;
  int hunger;
  int thirst;
  int warmth;
  int charm;
  int standing;
  std::vector<item> inventory;
};

extern playerStats player;

#endif
