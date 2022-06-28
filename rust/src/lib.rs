#![feature(default_free_fn)]

mod lists;
mod num;
mod trees;

pub(crate) use num::Num;

pub use lists::{
    AbsoluteCircularLinkedList, AbsoluteLinkedList, LinkedList, RelativeCircularLinkedList,
    RelativeLinkedList,
};

pub use trees::{NoRecurSzbTree, SzbTree};
