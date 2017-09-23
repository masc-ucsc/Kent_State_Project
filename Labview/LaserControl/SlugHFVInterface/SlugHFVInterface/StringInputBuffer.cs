using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SlugHFVInterface
{
    public class StringInputBuffer
    {
        private string buffer;
        private char newLineMarker;

        public StringInputBuffer(char nlm = '\n')
        {
            newLineMarker = nlm;
            buffer = "";
        }

        public string Update(string chunk)
        {
            buffer += chunk;

            if (buffer.Contains(newLineMarker)) {
                string[] split = buffer.Split(newLineMarker);
                buffer = (split.Length == 2) ? split[1] : "";
                return split[0];
            }
            else
                return null;
        }

        public string Buffer => buffer;
    }
}
