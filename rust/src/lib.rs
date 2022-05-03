#![feature(default_free_fn)]

mod lists;
mod trees;

pub use lists::{
    AbsoluteCircularDoublyLinkedList, AbsoluteDoublyLinkedListBase, DoublyLinkedListBase,
    RelativeCircularDoublyLinkedList, RelativeDoublyLinkedListBase,
};

pub use trees::{
    RecursionlessSizeBalancedTreeMethods, SizeBalancedTreeBase, SizeBalancedTreeMethods,
};
