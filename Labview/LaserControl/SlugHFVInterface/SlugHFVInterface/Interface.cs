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
        enum OperatingState { STATE_OFF, STATE_POWERING_UP, STATE_ON, };
        private OperatingState state = OperatingState.STATE_OFF;

        private StringInputBuffer ibuffer = new StringInputBuffer();

        private const string POWER_ON_CMD  = "onn";
        private const string POWER_OFF_CMD = "off";

        private const string VOLTAGE_LEVEL_FLAG    = "v";
        private const string DUTY_CYCLE_LEVEL_FLAG = "d";
        private const string PULSE_TIME_FLAG       = "p";

        private const int LIGHT_OFF = 0x333333FF;
        private const int LIGHT_POWERING = 0x00ddddFF;
        private const int LIGHT_ON = 0x00dd00FF;

        private bool fault = false;

        private DeviceSetting receivedSetting = new DeviceSetting();
        private DeviceSetting userSetting = new DeviceSetting();

        private Queue<string> cmdQueue = new Queue<string>();

        private int indicatorLight = LIGHT_OFF;

        public void Update(string serialIn, bool powerOnFlag, bool powerOffFlag, int power, int dutyCycle, int pulseTime)
        {
            string resp = ibuffer.Update(serialIn);

            if (resp != null)
                ProcessResponse(resp);

            UpdateUserSetting(powerOnFlag, powerOffFlag, power, dutyCycle, pulseTime);

            indicatorLight = UpdateIndicatorLight();
        }

        public void PopCommand()
        {
            cmdQueue.Dequeue();
        }

        private void ProcessResponse(string resp)            // update received power / duty cycle / pulse time
        {
            string trimmed = resp.Trim();
            if (trimmed.Length == 0)
                return;

            string[] chunks = trimmed.Split(';');

            if (chunks[0] == POWER_ON_CMD) {
                receivedSetting.On = true;
            } else if (chunks[0] == POWER_OFF_CMD) {
                receivedSetting.On = false;
            } else {
                fault = true;
            }

            int receivedPowerLevel, receivedDutyCycle, receivedPulseTime;

            if (int.TryParse(chunks[1], out receivedPowerLevel))
                receivedSetting.PowerLevel = receivedPowerLevel;
            else
                fault = true;

            if (int.TryParse(chunks[2], out receivedDutyCycle))
                receivedSetting.DutyCycle = receivedDutyCycle;
            else
                fault = true;

            if (int.TryParse(chunks[3], out receivedPulseTime))
                receivedSetting.PulseTime = receivedPulseTime;
            else
                fault = true;
        }

        private void UpdateUserSetting(bool powerOnFlag, bool powerOffFlag, int userPower, int userDutyCycle, int userPulseTime)
        {
            switch (state) {
                case OperatingState.STATE_OFF:
                    if (powerOnFlag)
                        state = OperatingState.STATE_POWERING_UP;

                    break;
                case OperatingState.STATE_POWERING_UP:
                    if (CheckSettingConsistency(userPower, userDutyCycle, userPulseTime))
                        cmdQueue.Enqueue(POWER_ON_CMD + "\n");

                    if (receivedSetting.On)
                        state = OperatingState.STATE_ON;

                    break;

                case OperatingState.STATE_ON:
                    if(powerOffFlag)
                        cmdQueue.Enqueue(POWER_OFF_CMD + "\n");

                    if (!receivedSetting.On)
                        state = OperatingState.STATE_OFF;

                    break;
            }

            if (userPower != userSetting.PowerLevel) {
                cmdQueue.Enqueue(PowerLevelCmd(userPower) + "\n");
                userSetting.PowerLevel = userPower;
            }

            if (userDutyCycle != userSetting.DutyCycle) {
                cmdQueue.Enqueue(DutyCycleCmd(userPower) + "\n");
                userSetting.DutyCycle = userDutyCycle;
            }

            if (userPulseTime != userSetting.PulseTime) {
                cmdQueue.Enqueue(PulseTimeCmd(userPower) + "\n");
                userSetting.PulseTime = userPulseTime;
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
            return VOLTAGE_LEVEL_FLAG + pl.ToString();
        }

        private string DutyCycleCmd(int pl)
        {
            return DUTY_CYCLE_LEVEL_FLAG + pl.ToString();
        }

        private string PulseTimeCmd(int pl)
        {
            return PULSE_TIME_FLAG + pl.ToString();
        }

        private bool CheckSettingConsistency(int userPower, int userDutyCycle, int userPulseTime)
        {
            return receivedSetting.PowerLevel == userPower && receivedSetting.DutyCycle == userDutyCycle && receivedSetting.PulseTime == userPulseTime;
        }

        public string OutMessage => cmdQueue.First();
        public bool HasOutMessage => cmdQueue.Count() > 0;

        public int Power => receivedSetting.PowerLevel;
        public int DutyCycle => receivedSetting.DutyCycle;
        public int PulseTime => 0;
        public bool IsOn => receivedSetting.On;
        public bool Fault => fault;
        public int IndicatorLight => indicatorLight;
    }
}
