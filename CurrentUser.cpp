#include "CurrentUser.h"

CurrentUser *CurrentUser::instance = nullptr;

CurrentUser::CurrentUser() {
    login = false;
    sellerLogin = false;
    user = nullptr;
}

CurrentUser *CurrentUser::getInstance() {
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

ID_t CurrentUser::getUserId() const {
    if (login)
        return user->id;
    return 0;
}

User CurrentUser::getUser() const {
    if (login)
        return *user;
    return User();
}

void CurrentUser::setUser(User user) {
    if (this->user) {
        delete this->user;
    }
    this->user = new User(user);
    login = true;
    sellerLogin = false;
}

Seller CurrentUser::getSeller() const {
    if (sellerLogin)
        return *dynamic_cast<Seller * const>(user);
    return Seller();
}

void CurrentUser::setSeller(Seller seller) {
    if (this->user) {
        delete this->user;
    }
    this->user = new Seller(seller);
    login = true;
    sellerLogin = true;
}

CurrentUser::~CurrentUser() {
    if (user) {
        delete user;
    }
    instance = nullptr;
}