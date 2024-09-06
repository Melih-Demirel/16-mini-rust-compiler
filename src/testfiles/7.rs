fn main()
{
    let mut a = 1;
    {
        println!(a);
        let a = 5;
        {
            println!(a);
        }
    }
}
