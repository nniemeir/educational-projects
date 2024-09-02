#ifndef LOCATIONS_H
#define LOCATIONS_H
#include "halfway.h"

class Location {
public:
  Location();
  Location(const QString &name, const QString &description,
           const QString &image, const std::vector<item> &inventory);

  // Location
  QString getName() const;
  QString getDescription() const;
  QString getImage() const;

  // Inventory
  int searchInventory(const QString &itemName) const;
  QString locInventory() const;
  const item &getInventoryItem(int index) const;
  void addItem(const item &itemToAdd);
  void removeItem(int index);
  QString getItemName(int index) const;
  int getItemEquipped(int index) const;
  void setItemEquipped(int index, int value);
  int getItemQuantity(int index) const;
  void setItemQuantity(int index, int value);

private:
  QString name;
  QString description;
  QString image;
  std::vector<item> inventory;
};

extern Location camp;
extern Location campPath;
extern Location cave;
extern Location caveEntrance;
extern Location intro;
extern Location lake;
extern Location valley;

#endif // LOCATIONS_H
