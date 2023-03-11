#include "User.h"
//Adiministrator
Adiministrator::Adiministrator() {
	Permission_level_ = 1;
}

//Member
Member::Member() {
	Permission_level_ = 2;
}

//Guest
Guest::Guest() {
	Permission_level_ = 3;
}
