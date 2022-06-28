use num_traits::{AsPrimitive, PrimInt};
use std::fmt::Debug;

pub trait Num: PrimInt + Debug + Default + AsPrimitive<usize> {}

impl<All: PrimInt + Debug + Default + AsPrimitive<usize>> Num for All {}
