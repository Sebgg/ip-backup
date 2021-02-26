using System.Windows;
using tddd49.Viewmodel;
using tddd49.Model;

namespace tddd49
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            DataContext = new ViewModel(new MainModel());

            InitializeComponent();
        }

    }
}
