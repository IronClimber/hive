import {
  CssBaseline,
  ThemeProvider,
  createTheme,
} from "@mui/material";
import { QueryClient, QueryClientProvider } from 'react-query'

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

const queryClient = new QueryClient()

function App() {
  return (
    <QueryClientProvider client={queryClient}>
    <ThemeProvider theme={theme}>
      <CssBaseline />
      <Header />
      <ControlPanel />
    </ThemeProvider>
    </QueryClientProvider>
  );
}

export default App;
