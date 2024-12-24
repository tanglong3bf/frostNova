import request from '@/utils/request'

export interface LoginForm {
  username: string;
  password: string;
}

export interface LoginResponse {
  token: string;
}

export const login = (username: string, password: string) => {
  return request.post<LoginResponse, LoginResponse, LoginForm>(
    '/login',
    {
      username,
      password
    })
}