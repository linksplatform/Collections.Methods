use num_traits::one;

use crate::LinkedList;
use crate::Num;

pub trait AbsoluteLinkedList<T: Num>: LinkedList<T> {
    fn get_first(&self) -> T;
    fn get_last(&self) -> T;
    fn get_size(&self) -> T;

    fn set_first(&mut self, element: T);
    fn set_last(&mut self, element: T);
    fn set_size(&mut self, size: T);

    fn inc_size(&mut self) {
        self.set_size(self.get_size() + one())
    }
    fn dec_size(&mut self) {
        self.set_size(self.get_size() - one())
    }
}
