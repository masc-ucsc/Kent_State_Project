using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SlugHFVInterface
{
    class DeviceSetting : UserSetting
    {
        public bool On { get; set; }
        public bool Fault { get; set; }

        public DeviceSetting(int pl, int dc, int pt, int ps, bool on, bool fault) : base(pl, dc, pt, ps)
        {
            On = on;
            Fault = fault;
        }

        public DeviceSetting(string deviceString)
        {
            string[] chunks = deviceString.Split(';');
            bool error = false;

            DeviceSetting settingBuffer = new DeviceSetting();

            if (chunks[0] == Interface.POWER_ON_CMD)
                settingBuffer.On = true;
            else if (chunks[0] == Interface.POWER_OFF_CMD)
                settingBuffer.On = false;
            else
                error = true;

            int receivedPowerLevel, receivedDutyCycle, receivedPulseTime, receivedPulseSpacing;

            if (int.TryParse(chunks[1], out receivedPowerLevel))
                PowerLevel = receivedPowerLevel;
            else
                error = true;

            if (int.TryParse(chunks[2], out receivedDutyCycle)) {
                if (receivedDutyCycle < 0 || receivedDutyCycle > 100)
                    receivedDutyCycle = 0;

                receivedDutyCycle = 256 * receivedDutyCycle / 100;      // cast to range of [0, 100]
                DutyCycle = receivedDutyCycle;
            }
            else
                error = true;

            if (int.TryParse(chunks[3], out receivedPulseTime))
                PulseTime = receivedPulseTime;
            else
                error = true;

            if (int.TryParse(chunks[4], out receivedPulseSpacing))
                PulseSpacing = receivedPulseSpacing;
            else
                error = true;

            if (error)
                Fault = true;
            else
                Fault = false;
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
