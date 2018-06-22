#ifndef CONT_H
#define CONT_H

template <class x>
class cont : public x {
	public:
		typedef typename x::container_type container_type;
		container_type &getContainer() {
			return this->c;
		}
};

#endif