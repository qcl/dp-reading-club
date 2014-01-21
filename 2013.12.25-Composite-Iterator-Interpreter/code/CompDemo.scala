// Composite pattern demo
//   using both late binding & pattern matching
object CompDemo {
  abstract class BST { def lookup(n: Int): Boolean }
  case object Leaf extends BST {
    def lookup(n: Int): Boolean = false
  }
  case class Node(lhs: BST, value: Int, rhs: BST) extends BST {
    def lookup(n: Int): Boolean =
      if (n < value) lhs.lookup(n)
      else if (n > value) rhs.lookup(n)
      else true
  }

  def insert(bst: BST, n: Int): BST = bst match {
    case Leaf => new Node(Leaf, n, Leaf)
    case Node(lhs, value, rhs) =>
      if (n < value)      new Node(insert(lhs, n), value, rhs)
      else if (n > value) new Node(lhs, value, insert(rhs, n))
      else                new Node(lhs, value, rhs)
  }

  def main(args: Array[String]): Unit = {
    val init = new Node(Leaf, 5, Leaf)
    val t1 = insert(init, 1)
    val t2 = insert(t1, 6)
    val t3 = insert(t2, 3)
    println(t3)
    println("lookup 3:" + t3.lookup(3))
    println("lookup 6:" + t3.lookup(6))
    println("lookup 2:" + t3.lookup(2))
  }
}
