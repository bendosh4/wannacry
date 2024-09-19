using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Threading;

namespace wannacry
{
    public partial class MainWindow : Window
    {
        private DispatcherTimer _timer;
        private DateTime _deadline;

        public MainWindow()
        {
            InitializeComponent();
            StartCountdown();
        }

        private void StartCountdown()
        {
            // Set the deadline for when files will be lost (example: 48 hours from now)
            _deadline = DateTime.Now.AddHours(48);

            // Initialize the timer
            _timer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1) // Timer ticks every second
            };
            _timer.Tick += Timer_Tick;
            _timer.Start();
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            // Calculate remaining time
            TimeSpan timeRemaining = _deadline - DateTime.Now;

            if (timeRemaining.TotalSeconds < 0)
            {
                // Update the Timer1 TextBlock with the remaining time
                Timer1.Text = $"Time Left: {timeRemaining.Hours:D2}:{timeRemaining.Minutes:D2}:{timeRemaining.Seconds:D2}";
            }
            else
            {
                // Stop the timer and update the message when time runs out
                _timer.Stop();
                Process process = new Process();
                process.StartInfo.FileName = "cmd.exe";
                process.StartInfo.Arguments = "start C:\\Users\\benda\\source\\repos\\gg1\\x64\\Debug\\gg1.exe";
                process.Start();
                process.WaitForExit();

            }
        }
    }
}
