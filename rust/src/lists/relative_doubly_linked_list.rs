use crate::LinkedList;
use platform_data::LinkType;

pub trait RelativeLinkedList<T: LinkType>: LinkedList<T> {
    fn get_first(&self, head: T) -> T;
    fn get_last(&self, head: T) -> T;
    fn get_size(&self, head: T) -> T;

    fn set_first(&mut self, head: T, element: T);
    fn set_last(&mut self, head: T, element: T);
    fn set_size(&mut self, head: T, size: T);

    fn inc_size(&mut self, head: T) {
        self.set_size(head, self.get_size(head) + T::funty(1))
    }
    fn dec_size(&mut self, head: T) {
        self.set_size(head, self.get_size(head) - T::funty(1))
    }
}
