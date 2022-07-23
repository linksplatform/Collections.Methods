// fixme: #![no_std]

mod lists;
mod trees;

pub use lists::{
    AbsoluteCircularLinkedList, AbsoluteLinkedList, LinkedList, RelativeCircularLinkedList,
    RelativeLinkedList,
};

pub use trees::{NoRecurSzbTree, SzbTree};
