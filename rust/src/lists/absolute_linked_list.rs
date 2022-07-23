use crate::LinkedList;
use platform_data::LinkType;

pub trait AbsoluteLinkedList<T: LinkType>: LinkedList<T> {
    fn get_first(&self) -> T;
    fn get_last(&self) -> T;
    fn get_size(&self) -> T;

    fn set_first(&mut self, element: T);
    fn set_last(&mut self, element: T);
    fn set_size(&mut self, size: T);

    fn inc_size(&mut self) {
        self.set_size(self.get_size() + T::funty(1))
    }
    fn dec_size(&mut self) {
        self.set_size(self.get_size() - T::funty(1))
    }
}
