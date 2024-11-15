#pragma once
#include "DataInterface.h"

class CurrentUser {
    public:
        static CurrentUser* getInstance();
        bool isLogin() const;
        bool isSeller() const;
        User getUser() const;
        void setUser(User user);
        Seller getSeller() const;
        void setSeller(Seller seller);
        ~CurrentUser();
    private:
        CurrentUser();
        static CurrentUser* instance;
        User* user;
        Seller* seller;
        bool login;
        bool sellerLogin;
};