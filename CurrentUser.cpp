#include "CurrentUser.h"

CurrentUser* CurrentUser::instance = nullptr;

CurrentUser::CurrentUser() {
    login = false;
    sellerLogin = false;
}

CurrentUser* CurrentUser::getInstance() {
    if (instance == nullptr) {
        instance = new CurrentUser();
    }
    return instance;
}

bool CurrentUser::isLogin() const {
    return login;
}

bool CurrentUser::isSeller() const {
    return sellerLogin;
}

User CurrentUser::getUser() const {
    if(login) return user;
    return User();
}

void CurrentUser::setUser(User user) {
    this->user = user;
    login = true;
    sellerLogin = false;
}

Seller CurrentUser::getSeller() const {
    if(sellerLogin) return seller;
    return Seller();
}

void CurrentUser::setSeller(Seller seller) {
    this->seller = seller;
    login = true;
    sellerLogin = true;
}