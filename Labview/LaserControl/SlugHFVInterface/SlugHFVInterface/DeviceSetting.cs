using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SlugHFVInterface
{
    class DeviceSetting
    {
        public int PowerLevel { get; set; }
        public int DutyCycle { get; set; }
        public int PulseTime { get; set; }
        public int PulseSpacing { get; set; }
        public bool On { get; set; }
        public bool Fault { get; set; }

        public DeviceSetting(int pl, int dc, int pt, int ps, bool on, bool fault)
        {
            PowerLevel = pl;
            DutyCycle = dc;
            PulseTime = pt;
            PulseSpacing = ps;
            On = on;
            Fault = fault;
        }

        public DeviceSetting()
        {
            PowerLevel = 0;
            DutyCycle = 0;
            PulseTime = 0;
            PulseSpacing = 0;
            On = false;
            Fault = false;
        }
    }
}
