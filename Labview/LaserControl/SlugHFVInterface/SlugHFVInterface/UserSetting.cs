using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SlugHFVInterface
{
    class UserSetting
    {
        public int PowerLevel { get; set; }
        public int DutyCycle { get; set; }
        public int PulseTime { get; set; }
        public int PulseSpacing { get; set; }

        public UserSetting(int powerLevel, int dutyCycle, int pulseTime, int pulseSpacing)
        {
            PowerLevel = powerLevel;
            DutyCycle = dutyCycle;
            PulseTime = pulseTime;
            PulseSpacing = pulseSpacing;
        }

        public UserSetting()
        {
            PowerLevel = 0;
            DutyCycle = 0;
            PulseTime = 0;
            PulseSpacing = 0;
        }
    }
}
