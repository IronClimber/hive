import {
  CssBaseline,
  ThemeProvider,
  createTheme,
} from "@mui/material";

import { Charts } from "./Charts";
import { ControlPanel } from "./ControlPanel";
import { Header} from './Header'

const theme = createTheme({
  components: {
    MuiIcon: {
      styleOverrides: {
        root: {
          // Match 24px = 3 * 2 + 1.125 * 16
          boxSizing: 'content-box',
          padding: 3,
          fontSize: '1.125rem',
        },
      },
    },
  },
});

function App() {
  return (
    <ThemeProvider theme={theme}>
      <CssBaseline />
      <Header />
      <ControlPanel />
      <Charts />
    </ThemeProvider>
  );
}

export default App;
