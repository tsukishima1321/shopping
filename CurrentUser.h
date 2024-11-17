#pragma once
#include "Types.h"

class CurrentUser {
public:
    static CurrentUser *getInstance();
    bool isLogin() const;
    bool isSeller() const;
    ID_t getUserId() const;
    User getUser() const;
    void setUser(User user);
    Seller getSeller() const;
    void setSeller(Seller seller);
    ~CurrentUser();

private:
    CurrentUser();
    static CurrentUser *instance;
    User *user;
    bool login;
    bool sellerLogin;
};