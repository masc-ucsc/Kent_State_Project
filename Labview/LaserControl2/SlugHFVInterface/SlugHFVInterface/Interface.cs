using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;


namespace SlugHFVInterface
{
    public class Interface
    {
        enum OperatingState { STATE_OFF, STATE_POWERING_UP, STATE_ON };
        private OperatingState state = OperatingState.STATE_OFF;

        private StringInputBuffer ibuffer = new StringInputBuffer();

        public const string POWER_ON_CMD  = "onn";
        public const string POWER_OFF_CMD = "off";

        private const string VOLTAGE_LEVEL_FLAG    = "v";
        private const string DUTY_CYCLE_LEVEL_FLAG = "d";
        private const string PULSE_TIME_FLAG       = "p";
        private const string PULSE_SPACING_FLAG    = "s";

        private const int LIGHT_OFF = 0x333333FF;
        private const int LIGHT_POWERING = 0x00ddddFF;
        private const int LIGHT_ON = 0x00dd00FF;

        private bool fault = false;

        private DeviceSetting receivedSetting = new DeviceSetting();
        private DeviceSetting userSetting = new DeviceSetting();

        private Queue<string> cmdQueue = new Queue<string>();

        private int indicatorLight = LIGHT_OFF;

        public void Update(string serialIn, bool powerOnFlag, bool powerOffFlag, int power, int dutyCycle, int pulseTime, int pulseSpacing)
        {
            string resp = ibuffer.Update(serialIn);

            if (resp != null)
                receivedSetting = new DeviceSetting(resp);

            UpdateUserSetting(powerOnFlag, powerOffFlag, new UserSetting(power, dutyCycle, pulseTime, pulseSpacing));
            indicatorLight = UpdateIndicatorLight();
        }

        public void PopCommand()
        {
            cmdQueue.Dequeue();
        }

        private void UpdateUserSetting(bool powerOnFlag, bool powerOffFlag, UserSetting newSetting)
        {
            switch (state) {
                case OperatingState.STATE_OFF:
                    if (powerOnFlag)
                        state = OperatingState.STATE_POWERING_UP;

                    break;
                case OperatingState.STATE_POWERING_UP:
                    if (CheckSettingConsistency(newSetting))
                        cmdQueue.Enqueue(POWER_ON_CMD);

                    if (receivedSetting.On)
                        state = OperatingState.STATE_ON;

                    break;

                case OperatingState.STATE_ON:
                    if(powerOffFlag)
                        cmdQueue.Enqueue(POWER_OFF_CMD);

                    if (!receivedSetting.On)
                        state = OperatingState.STATE_OFF;

                    break;
            }

            if (newSetting.PowerLevel != userSetting.PowerLevel) {
                cmdQueue.Enqueue(PowerLevelCmd(newSetting.PowerLevel));
                userSetting.PowerLevel = newSetting.PowerLevel;
            }

            if (newSetting.DutyCycle != userSetting.DutyCycle) {
                cmdQueue.Enqueue(DutyCycleCmd(newSetting.DutyCycle));
                userSetting.DutyCycle = newSetting.DutyCycle;
            }

            if (newSetting.PulseTime != userSetting.PulseTime) {
                cmdQueue.Enqueue(PulseTimeCmd(newSetting.PulseTime));
                userSetting.PulseTime = newSetting.PulseTime;
            }

            if (newSetting.PulseSpacing != userSetting.PulseSpacing) {
                cmdQueue.Enqueue(PulseSpacingCmd(newSetting.PulseSpacing));
                userSetting.PulseSpacing = newSetting.PulseSpacing;
            }
        }

        private int UpdateIndicatorLight()
        {
            if (state == OperatingState.STATE_OFF)
                return LIGHT_OFF;
            else if (state == OperatingState.STATE_POWERING_UP)
                return LIGHT_POWERING;
            else
                return LIGHT_ON;
        }

        private string PowerLevelCmd(int pl)
        {
            return VOLTAGE_LEVEL_FLAG + pl.ToString() + "\n";
        }

        private string DutyCycleCmd(int pl)
        {
            return DUTY_CYCLE_LEVEL_FLAG + pl.ToString() + "\n";
        }

        private string PulseTimeCmd(int pl)
        {
            return PULSE_TIME_FLAG + pl.ToString() + "\n";
        }

        private string PulseSpacingCmd(int pl)
        {
            return PULSE_SPACING_FLAG + pl.ToString() + "\n";
        }

        private bool CheckSettingConsistency(UserSetting other)
        {
            return receivedSetting.PowerLevel == other.PowerLevel && receivedSetting.DutyCycle == other.DutyCycle &&
                receivedSetting.PulseTime == other.PulseTime && receivedSetting.PulseSpacing == other.PulseSpacing;
        }

        public string OutMessage => cmdQueue.First();
        public bool HasOutMessage => cmdQueue.Count() > 0;
        public int MessageCount => cmdQueue.Count();

        public int Power => receivedSetting.PowerLevel;
        public int DutyCycle => receivedSetting.DutyCycle;
        public int PulseTime => receivedSetting.PulseTime;
        public int PulseSpacing => receivedSetting.PulseSpacing;
        public bool IsOn => receivedSetting.On;
        public bool Fault => fault;
        public int IndicatorLight => indicatorLight;
    }
}
