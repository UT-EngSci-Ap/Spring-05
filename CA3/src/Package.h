#pragma once
#include <vector>
#include <string>
#include "Installable.h"

class Package : public Installable {
private:
    std::vector<Installable*> children;

public:
    Package(std::string id, std::string title);

    bool isPackage() const override;

    void addChild(Installable* child);
    bool hasChild(const std::string& childId) const;

    bool install(TransactionContext& tx) override;
    void uninstall() override;
};
