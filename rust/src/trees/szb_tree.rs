use platform_data::LinkType;

pub trait SzbTree<T: LinkType> {
    unsafe fn get_mut_left_reference(&mut self, node: T) -> *mut T;

    unsafe fn get_mut_right_reference(&mut self, node: T) -> *mut T;

    unsafe fn get_left_reference(&self, node: T) -> *const T;

    unsafe fn get_right_reference(&self, node: T) -> *const T;

    unsafe fn get_left(&self, node: T) -> T;

    unsafe fn get_right(&self, node: T) -> T;

    unsafe fn get_size(&self, node: T) -> T;

    unsafe fn set_left(&mut self, node: T, left: T);

    unsafe fn set_right(&mut self, node: T, right: T);

    unsafe fn set_size(&mut self, node: T, size: T);

    unsafe fn first_is_to_the_left_of_second(&self, first: T, second: T) -> bool;

    unsafe fn first_is_to_the_right_of_second(&self, first: T, second: T) -> bool;

    unsafe fn get_left_or_default(&self, node: T) -> T {
        if node == T::funty(0) {
            T::funty(0)
        } else {
            self.get_left(node)
        }
    }

    unsafe fn get_right_or_default(&self, node: T) -> T {
        if node == T::funty(0) {
            T::funty(0)
        } else {
            self.get_right(node)
        }
    }

    unsafe fn get_size_or_zero(&self, node: T) -> T {
        if node == T::funty(0) {
            T::funty(0)
        } else {
            self.get_size(node)
        }
    }

    unsafe fn inc_size(&mut self, node: T) {
        self.set_size(node, self.get_size(node) + T::funty(1));
    }

    unsafe fn dec_size(&mut self, node: T) {
        self.set_size(node, self.get_size(node) - T::funty(1));
    }

    unsafe fn get_left_size(&self, node: T) -> T {
        self.get_size_or_zero(self.get_left_or_default(node))
    }

    unsafe fn get_right_size(&self, node: T) -> T {
        self.get_size_or_zero(self.get_right_or_default(node))
    }

    unsafe fn fix_size(&mut self, node: T) {
        self.set_size(
            node,
            (self.get_left_size(node) + self.get_right_size(node)) + T::funty(1),
        );
    }

    unsafe fn left_rotate(&mut self, root: *mut T) {
        *root = self.left_rotate_core(*root);
    }

    unsafe fn left_rotate_core(&mut self, root: T) -> T {
        let right = self.get_right(root);
        self.set_right(root, self.get_left(right));
        self.set_left(right, root);
        self.set_size(right, self.get_size(root));
        self.fix_size(root);
        right
    }

    unsafe fn right_rotate(&mut self, root: *mut T) {
        *root = self.right_rotate_core(*root);
    }

    unsafe fn right_rotate_core(&mut self, root: T) -> T {
        let left = self.get_left(root);
        self.set_left(root, self.get_right(left));
        self.set_right(left, root);
        self.set_size(left, self.get_size(root));
        self.fix_size(root);
        left
    }

    unsafe fn get_rightest(&self, mut current: T) -> T {
        let mut current_right = self.get_right(current);
        while current_right != T::funty(0) {
            current = current_right;
            current_right = self.get_right(current);
        }
        current
    }

    unsafe fn get_leftest(&self, mut current: T) -> T {
        let mut current_left = self.get_left(current);
        while current_left != T::funty(0) {
            current = current_left;
            current_left = self.get_left(current);
        }
        current
    }

    unsafe fn get_next(&self, node: T) -> T {
        self.get_leftest(self.get_right(node))
    }

    unsafe fn get_previous(&self, node: T) -> T {
        self.get_rightest(self.get_left(node))
    }

    unsafe fn contains(&self, node: T, mut root: T) -> bool {
        while root != T::funty(0) {
            if self.first_is_to_the_left_of_second(node, root) {
                root = self.get_left(root);
            } else if self.first_is_to_the_right_of_second(node, root) {
                root = self.get_right(root);
            } else {
                return true;
            }
        }
        false
    }

    unsafe fn clear_node(&mut self, node: T) {
        self.set_left(node, T::funty(0));
        self.set_right(node, T::funty(0));
        self.set_size(node, T::funty(0));
    }
}
